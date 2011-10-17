/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.
Para maiores informacoes:
lince@dc.ufscar.br
mgp@icmc.usp.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
http://www.icmc.usp.br/php/laboratorio.php?origem=icmc&id_lab=3&nat=icmc
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
lince@dc.ufscar.br
mgp@icmc.usp.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
*******************************************************************************/

/**
* @file EnhancedInputManager.cpp
* @author Diogo de Carvalho Pedrosa
* @author Caio Viel
* @author José Augusto Costa Martins Júnior
* @date 03-09-10
*/

/* Includes específicos do enhanced */
#include "../include/EnhancedInputManager.h"
#include "../include/DFBMultimodalInputEvent.h"
#include "../include/MultimodalInputEvent.h"

#include "../include/EventParser.h"
#include "../include/CommunicationManager.h"

/* Includes herdados do inputmanager */

#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;


namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
	// TODO Ler o botão de mudança de modo de um arquivo.
	// TODO Tentar separar o tratamento de mudanças de modo e modos de entrada
	// para outras classes, modularizando esta.
	static IComponentManager* cm = IComponentManager::getCMInstance();

	EnhancedInputManager* EnhancedInputManager::_instance = NULL;
	int EnhancedInputManager::KEY_MODE_CHANGE = 61968;

	EnhancedInputManager::EnhancedInputManager() {

		// Cópiado do construtor de InputManager

		eventListeners         = new map<IInputEventListener*, set<int>*>;
		actionsToInpListeners  = new vector<LockedAction*>;

		applicationListeners   = new set<IInputEventListener*>;
		actionsToAppListeners  = new vector<LockedAction*>;

		currentXAxis  = 0;
		currentYAxis  = 0;
		maxX          = 0;
		maxY          = 0;
		lastEventTime = 0;

		eventBuffer   = ((EventBufferCreator*)(cm->getObject("EventBuffer")))();

		running       = true;
		notifying     = false;
		notifyingApp  = false;

		pthread_mutex_init(&actInpMutex, NULL);
		pthread_mutex_init(&actAppMutex, NULL);

		pthread_mutex_init(&appMutex, NULL);

		// Para tratamento do InputMode

		inputModes = new vector<IInputMode*>();
		inputModeMap = new map<string, IInputMode*>();
		inputModeIndex = -1;
		currentInputMode = NULL;
		logger = LoggerUtil::getLogger(
				"br.ufscar.lince.ginga.core.mmi.EnhancedInputManager");
		LoggerUtil_info(logger, "Construtor");

		// Para tratamento de eventos multimodais.

		multimodalEventListeners = new set<IMultimodalInputEventListener*>;
		actionsToMultimodalListeners = new vector<LockedMultimodalAction*>;

		notifyingMultimodal = false;

		pthread_mutex_init(&multimodalMutex, NULL);
		pthread_mutex_init(&actMultimodalMutex, NULL);

		// Para manipulação de teclas

		codeMap = CodeMap::getInstance();

		// TODO Prover uma forma que permita que uma aplicação residente de
		// configuração do STB decida se o serviço de entrada de dados
		// multimodais deve ser iniciado ou não.
		CommunicationManager *communicationManager =
				CommunicationManager::getInstance();
		communicationManager->start();

		initializeInputIntervalTime();

		// Inicia o laço de tratamento de eventos.
		EnhancedInputManager::start();
	}

	EnhancedInputManager* EnhancedInputManager::getInstance() {
		
		if (_instance == NULL) {
			_instance = new EnhancedInputManager();
		}
		return _instance;
	}

	EnhancedInputManager::~EnhancedInputManager() {
		LoggerUtil_info(logger, "bool EnhancedInputManager::"
				"~EnhancedInputManager()");

		if (_instance != NULL) {
			delete _instance;
			_instance = NULL;
		}
	}

	void EnhancedInputManager::initializeInputIntervalTime() {
		string strVar;
		ifstream fis;

		imperativeIntervalTime = 0;
		declarativeIntervalTime = 0;

		strVar = "/usr/local/etc/ginga/files/system/config/input.cfg";
		fis.open(strVar.c_str(), ifstream::in);
		if (!fis.is_open()) {
			LoggerUtil_info(logger, "initializeInputIntervalTime(): can't open "
					"input file: " << strVar.c_str());
			return;
		}

		while (fis.good()) {
			fis >> strVar;
			if (strVar == "imperative.inputIntervalTime" && fis.good()) {
				fis >> strVar;
				if (strVar == "=" && fis.good()) {
					fis >> strVar;
					if (strVar != "") {
						imperativeIntervalTime = stof(strVar);
					}
				}
			} else if (strVar == "declarative.inputIntervalTime" &&
					fis.good()) {

				fis >> strVar;
				if (strVar == "=" && fis.good()) {
					fis >> strVar;
					if (strVar != "") {
						declarativeIntervalTime = stof(strVar);
					}
				}
			}
		}
		fis.close();

		LoggerUtil_info(logger, "initializeInputIntervalTime()" <<
				endl << "imperative = " << imperativeIntervalTime << endl <<
				"declarative = " << declarativeIntervalTime);
	}

	// Esta tambem e uma funcao herdada que nao era virtual (demais estao la em
	// baixo), so que precisamos fazer outras coisas tambem.
	void EnhancedInputManager::release() {
		running = false;
		if (eventBuffer != NULL) {
			eventBuffer->wakeUp();
		}
		lock();
		notifying = true;
		LoggerUtil_info(logger, "release");

		if (eventListeners != NULL) {
			eventListeners->clear();
			delete eventListeners;
			eventListeners = NULL;
		}

		pthread_mutex_lock(&actInpMutex);
		if (actionsToInpListeners != NULL) {
			actionsToInpListeners->clear();
			delete actionsToInpListeners;
			actionsToInpListeners = NULL;
		}
		pthread_mutex_unlock(&actInpMutex);
		pthread_mutex_destroy(&actInpMutex);

		pthread_mutex_lock(&appMutex);
		notifyingApp = true;

		if (applicationListeners != NULL) {
			applicationListeners->clear();
			delete applicationListeners;
			applicationListeners = NULL;
		}

		pthread_mutex_lock(&actAppMutex);
		if (actionsToAppListeners != NULL) {
			actionsToAppListeners->clear();
			delete actionsToAppListeners;
			actionsToAppListeners = NULL;
		}
		pthread_mutex_unlock(&actAppMutex);
		pthread_mutex_destroy(&actAppMutex);

		pthread_mutex_unlock(&appMutex);
		pthread_mutex_destroy(&appMutex);

		if (eventBuffer != NULL) {
			delete eventBuffer;
			eventBuffer = NULL;
		}

		unlock();

		// Libera tudo relacionado a eventos multimodais.
		pthread_mutex_lock(&multimodalMutex);
		notifyingMultimodal = true;

		if (multimodalEventListeners != NULL) {
			multimodalEventListeners->clear();
			delete multimodalEventListeners;
			multimodalEventListeners = NULL;
		}

		pthread_mutex_lock(&actMultimodalMutex);
		if (actionsToMultimodalListeners != NULL) {
			actionsToMultimodalListeners->clear();
			delete actionsToMultimodalListeners;
			actionsToMultimodalListeners = NULL;
		}
		pthread_mutex_unlock(&actMultimodalMutex);
		pthread_mutex_destroy(&actMultimodalMutex);

		pthread_mutex_unlock(&multimodalMutex);
		pthread_mutex_destroy(&multimodalMutex);
	}

	void EnhancedInputManager::addInputEventListener(
			IInputEventListener* listener, set<int>* evs) {

		LockedAction* action;
		set<int>* iEvs;
		map<IInputEventListener*, set<int>*>::iterator i;

		if (!running) {
			return;
		}

		if (notifying) {
			pthread_mutex_lock(&actInpMutex);
			action = new LockedAction;
			action->l      = listener;
			action->isAdd  = true;
			action->events = evs;

			actionsToInpListeners->push_back(action);
			pthread_mutex_unlock(&actInpMutex);

		} else {
			lock();
			performInputLockedActions();
			i = eventListeners->find(listener);
			if (i != eventListeners->end()) {
				iEvs = i->second;
				eventListeners->erase(i);
				if (iEvs != NULL) {
					delete iEvs;
				}
			}
			(*eventListeners)[listener] = evs;
			unlock();
		}
	}

	void EnhancedInputManager::removeInputEventListener(
			IInputEventListener* listener) {

		LockedAction* action;
		map<IInputEventListener*, set<int>*>::iterator i;

		if (!running) {
			return;
		}

		if (notifying) {
			pthread_mutex_lock(&actInpMutex);
			action = new LockedAction;
			action->l      = listener;
			action->isAdd  = false;
			action->events = NULL;

			actionsToInpListeners->push_back(action);
			pthread_mutex_unlock(&actInpMutex);

		} else {
			lock();
			if (!running) {
				unlock();
				return;
			}
			performInputLockedActions();
			i = eventListeners->find(listener);
			if (i != eventListeners->end()) {
				if (i->second != NULL) {
					delete i->second;
				}
				eventListeners->erase(i);
			}
			unlock();
		}
	}

	void EnhancedInputManager::performInputLockedActions() {
		LockedAction* action;
		IInputEventListener* listener;

		set<int>* evs;
		map<IInputEventListener*, set<int>*>::iterator i;
		vector<LockedAction*>::iterator j;

		if (!running) {
			return;
		}

		pthread_mutex_lock(&actInpMutex);
		j = actionsToInpListeners->begin();
		while (j != actionsToInpListeners->end()) {
			action   = *j;
			listener = action->l;
			evs      = action->events;

			i = eventListeners->find(listener);
			if (i != eventListeners->end()) {
				if (i->second != NULL) {
					delete i->second;
				}
				eventListeners->erase(i);
			}

			if (action->isAdd) {
				(*eventListeners)[listener] = evs;
			}

			delete action;
			++j;
		}

		actionsToInpListeners->clear();
		pthread_mutex_unlock(&actInpMutex);
	}

	void EnhancedInputManager::performApplicationLockedActions() {
		LockedAction* action;
		IInputEventListener* listener;

		set<IInputEventListener*>::iterator i;
		vector<LockedAction*>::iterator j;

		if (!running) {
			return;
		}

		pthread_mutex_lock(&actAppMutex);
		j = actionsToAppListeners->begin();
		while (j != actionsToAppListeners->end()) {
			action = *j;
			listener = action->l;

			if (action->isAdd) {
				applicationListeners->insert(listener);

			} else {
				i = applicationListeners->find(listener);
				if (i != applicationListeners->end()) {
					applicationListeners->erase(i);
				}
			}

			delete action;
			++j;
		}

		actionsToAppListeners->clear();
		pthread_mutex_unlock(&actAppMutex);
	}

	bool EnhancedInputManager::dispatchEvent(IInputEvent* inputEvent) {
		map<IInputEventListener*, set<int>*>::iterator i;

		set<int>* evs;
		int keyCode;

		if (!running) {
			return false;
		}

		notifying = true;
		lock();
		notifying = true;

		performInputLockedActions();

		if (eventListeners->empty() || inputEvent == NULL) {
			LoggerUtil_info(logger, "dispatchEvent Warning! No listeners found "
					"or inputEvent is null");
			unlock();
			notifying = false;
			return true;
		}

		keyCode = inputEvent->getKeyCode();
		i = eventListeners->begin();
		while (i != eventListeners->end()) {
			evs = i->second;
			if (evs != NULL) {
				if (evs->find(keyCode) != evs->end()) {
					//return false means an event with changed keySymbol
					if (!i->first->userEventReceived(inputEvent)) {
						unlock();
						notifying = false;
						return false;
					}
				}

			} else if (!i->first->userEventReceived(inputEvent)) {
				unlock();
				notifying = false;
				return false;
			}
			++i;
		}
		unlock();
		notifying = false;
		return true;
	}

	bool EnhancedInputManager::dispatchApplicationEvent(IInputEvent* inputEvent) {
		set<IInputEventListener*>::iterator i;
		vector<LockedAction*>::iterator j;

		if (!running) {
			return false;
		}

		notifyingApp = true;
		pthread_mutex_lock(&appMutex);

		performApplicationLockedActions();

		if (applicationListeners->empty() || inputEvent == NULL) {
			pthread_mutex_unlock(&appMutex);
			notifyingApp = false;
			return true;
		}

		i = applicationListeners->begin();
		while (i != applicationListeners->end()) {
			if (*i != NULL) {
				if (!(*i)->userEventReceived(inputEvent)) {
					pthread_mutex_unlock(&appMutex);
					notifyingApp = false;
					return false;
				}
			}
			++i;
		}
		pthread_mutex_unlock(&appMutex);
		notifyingApp = false;
		return true;
	}

	void EnhancedInputManager::addApplicationInputEventListener(
			IInputEventListener* listener) {

		LockedAction* action;

		if (!running) {
			return;
		}

		if (notifyingApp) {
			pthread_mutex_lock(&actAppMutex);
			action         = new LockedAction;
			action->l      = listener;
			action->isAdd  = true;
			action->events = NULL;

			actionsToAppListeners->push_back(action);
			pthread_mutex_unlock(&actAppMutex);

		} else {
			pthread_mutex_lock(&appMutex);
			performApplicationLockedActions();
			applicationListeners->insert(listener);
			pthread_mutex_unlock(&appMutex);
		}
	}

	void EnhancedInputManager::removeApplicationInputEventListener(
			IInputEventListener* listener) {

		LockedAction* action;
		set<IInputEventListener*>::iterator i;

		if (!running) {
			return;
		}

		if (notifyingApp) {
			pthread_mutex_lock(&actAppMutex);
			action         = new LockedAction;
			action->l      = listener;
			action->isAdd  = false;
			action->events = NULL;

			actionsToAppListeners->push_back(action);
			pthread_mutex_unlock(&actAppMutex);

		} else {
			pthread_mutex_lock(&appMutex);
			performApplicationLockedActions();
			i = applicationListeners->find(listener);
			if (i != applicationListeners->end()) {
				applicationListeners->erase(i);
			}
			pthread_mutex_unlock(&appMutex);
		}
	}

	void EnhancedInputManager::postEvent(IInputEvent* event) {
		LoggerUtil_info(logger, "postEvent(IInputEvent* event)");

		if (!running) {
			return;
		}

		eventBuffer->postEvent(event);
	}

	void EnhancedInputManager::postEvent(int keyCode) {
		IInputEvent* ie;
		ie = ((InputEventCreator*)(cm->getObject("InputEvent")))(NULL, keyCode);
		postEvent(ie);
	}

	void EnhancedInputManager::setAxisValues(int x, int y, int z) {
		this->currentXAxis = x;
		this->currentYAxis = y;
	}

	void EnhancedInputManager::setAxisBoundaries(int x, int y, int z) {
		this->maxX = x;
		this->maxY = y;
	}

	int EnhancedInputManager::getCurrentXAxisValue() {
		return currentXAxis;
	}

	int EnhancedInputManager::getCurrentYAxisValue() {
		return currentYAxis;
	}

	// Esse método foi copiado do InputManager e alterado para dar suporte a
	// eventos do tipo MultimodalInputEvent.
	void EnhancedInputManager::run() {
		IInputEvent* inputEvent;
		int pLastCode = -1;
		int lastCode = -1;
		double pTimeStamp = 0;
		LoggerUtil_info(logger, "run()");

		while (running && eventBuffer != NULL) {
			eventBuffer->waitEvent();
			if (!running) {
				break;
			}

			inputEvent = eventBuffer->getNextEvent();
			while (inputEvent != NULL) {
				if (inputEvent->isMotionType()) {
					inputEvent->getAxisValue(
							&currentXAxis, &currentYAxis, NULL);

					/*cout << "InputManager::run got currentX = '";
					cout << currentXAxis << "' currentY = '";
					cout << currentYAxis << "'" << endl;*/

					if (currentXAxis > maxX) {
						currentXAxis = maxX;
					}

					if (currentYAxis > maxY) {
						currentYAxis = maxY;
					}

					/*cout << "InputManager::run new currentX = '";
					cout << currentXAxis << "' currentY = '";
					cout << currentYAxis << "'" << endl;*/

					delete inputEvent;
					inputEvent = eventBuffer->getNextEvent();
					continue;
				}

				// Pressionamento de botão do mouse.
				if (inputEvent->isButtonPressType()) {
					inputEvent->setAxisValue(currentXAxis, currentYAxis, 0);
					dispatchEvent(inputEvent);
					delete inputEvent;
					inputEvent = eventBuffer->getNextEvent();
					continue;
				}

				// Pressionamento de tecla do controle remoto ou teclado
				// Acrescentei esse if para permitir sair do ginga recebendo
				// a mensagem MAIN ALL DONE e, ao mesmo tempo, permitir que o
				// ginga permaneça em execução mesmo se não existir uma
				// aplicação ncl em execução e não estiver ocorrendo o
				// recebimento de um ts.
				if (inputEvent->isPressedType() &&
						(inputEvent->getKeyCode() == CodeMap::KEY_POWER ||
						inputEvent->getKeyCode() == CodeMap::KEY_F10)) {

					unlockConditionSatisfied();
					inputEvent = eventBuffer->getNextEvent();
					continue;
				}

				// Pressionamento de tecla do controle remoto ou teclado
				if (inputEvent->isPressedType() && ((getCurrentTimeMillis() -
						timeStamp) >= declarativeIntervalTime ||
								lastCode != inputEvent->getKeyCode())) {

					lastCode = inputEvent->getKeyCode();
					timeStamp = getCurrentTimeMillis();
					
					if (inputEvent->getKeyCode() == KEY_MODE_CHANGE) {
						nextInputMode();
						delete inputEvent;
						inputEvent = eventBuffer->getNextEvent();
						continue;
					}

					if (currentInputMode != NULL &&
							currentInputMode->getInputModeType() ==
							IInputMode::REDIRECTER) {

						((InputModeRedirecter*) currentInputMode)->
								userEventReceived(inputEvent);

						delete inputEvent;
						inputEvent = eventBuffer->getNextEvent();
						continue;

					} else if (currentInputMode != NULL &&
							currentInputMode->getInputModeType() == 
							IInputMode::MODIFIER) {

						IInputEvent* newEvent;
						newEvent = ((InputModeModifier*) currentInputMode)->
								modifierEvent(inputEvent);
						inputEvent = newEvent;
					}
					
					if (!dispatchEvent(inputEvent)) {
						delete inputEvent;
						inputEvent = eventBuffer->getNextEvent();
						continue;
					}
				}

				// Eventos multimodais chegam aqui com a class User.
				if (inputEvent->isUserClass()) {
					DFBUserEvent* dfbEvent =
							(DFBUserEvent*)inputEvent->getContent();

					MultimodalInputEvent* mie =
							(MultimodalInputEvent*) dfbEvent->data;

					if (!dispatchMultimodalEvent(mie)) {
						delete inputEvent;
						delete mie;
						mie = NULL;
						inputEvent = eventBuffer->getNextEvent();
						continue;
					} else {
						// Tenho que colocar no if e no else por causa do
						// continue que está dentro do if.
						delete mie;
						mie = NULL;
					}
				}

				// Pressionamento ou liberação de tecla do controle remoto ou
				// teclado
				if ((inputEvent->isKeyType() && ((getCurrentTimeMillis() -
						pTimeStamp) >= imperativeIntervalTime ||
								pLastCode != inputEvent->getKeyCode())) ||
								inputEvent->isUserClass()) {

					pLastCode = inputEvent->getKeyCode();
					pTimeStamp = getCurrentTimeMillis();
					dispatchApplicationEvent(inputEvent);
				}

				delete inputEvent;
				inputEvent = eventBuffer->getNextEvent();
			}
		}

	}

	void EnhancedInputManager::addInputMode(
				string inputModeId, IInputMode* inputMode) {

		LoggerUtil_info(logger, "addInputMode(string inputModeId,"
				"IInputMode* inputMode): " << inputModeId);
		if (inputMode == NULL) {
			string men = "bool EnhancedInputManager::addInputMode(";
			men += "string inputModeId, IInputMode* inputMode) \n";
			men += "inputMode é nulo.";
			throw BadArgumentException(men);
		}
		
		map<string, IInputMode*>::iterator it;
		it = inputModeMap->find(inputModeId);

		if (it != inputModeMap->end()) {
			string men = "bool EnhancedInputManager::addInputMode(";
			men += "string inputModeId, IInputMode* inputMode) \n";
			men += "Modo de entrada com identificador " + inputModeId +
					" já adicionado.";
			throw DuplicateException(men);
		}

		(*inputModeMap)[inputModeId] = inputMode;
		inputModes->push_back(inputMode);
	}

	IInputMode* EnhancedInputManager::removeInputMode(string inputModeId) {
		LoggerUtil_info(logger, "removeInputMode(string inputModeId): " <<
				inputModeId);

		map<string, IInputMode*>::iterator it;
		it = inputModeMap->find(inputModeId);
		IInputMode* inputMode = it->second;
		vector<IInputMode*>::iterator it2;
		it2 = inputModes->begin();
		while (it2 != inputModes->end()) {
			if (*it2 == inputMode) {
				break;
			}
			it2++;
		}

		if (it == inputModeMap->end() ||
			it2 == inputModes->end()) {
			return NULL;
		}
		inputModeMap->erase(it);
		inputModes->erase(it2);
		if (inputMode == currentInputMode) {
			string men = "IInputMode* EnhancedInputManager::removeInputMode("
					"string inputModeId)\n";
			men += "Tentativa de remover o modo de entrada atual.";
			throw BadArgumentException(men);
		}
		return inputMode;
	}

	IInputMode* EnhancedInputManager::getInputMode(string inputModeId) {
		LoggerUtil_info(logger, "getInputMode(string inputModeId): " <<
				inputModeId);

		map<string, IInputMode*>::iterator it;
		it = inputModeMap->find(inputModeId);

		if (it == inputModeMap->end()) {
			return NULL;
		}
		return it->second;
	}

	void EnhancedInputManager::gotoInputMode(string inputModeId) {
		LoggerUtil_info(logger, "gotoInputMode(string inputModeId): " <<
				inputModeId);

		IInputMode* inputMode = getInputMode(inputModeId);
		if (inputMode == NULL) {
			string men = "void EnhancedInputManager::gotoInputMode("
					"string inputModeId)";
			men += "Modo de entrada solicitado não existe.";
			throw BadArgumentException(men);
		}
		currentInputMode = inputMode;
		int index = 0;
		for (; index < inputModes->size(); index++) {
			if (inputMode == inputModes->at(index)) {
				break;
			}
		}
		inputModeIndex = index;
	}

	void EnhancedInputManager::nextInputMode() {
		LoggerUtil_info(logger, "nextInputMode()");

		int modeCount = (int) inputModes->size();
		if (modeCount == 0) {
			LoggerUtil_warning(logger, "Nenhum modo para ser selecionado!");
		} else {
			LoggerUtil_info(logger, "Vamos mudar de modo de entrada");
			inputModeIndex++;
			if (inputModeIndex < inputModes->size()) {
				if (currentInputMode != NULL) {
					currentInputMode->exitingInputMode();
				}
				currentInputMode = inputModes->at(inputModeIndex);
				currentInputMode->startingInputMode();
			} else {
				if (currentInputMode != NULL) {
						currentInputMode->exitingInputMode();
				}
				inputModeIndex = -1;
				currentInputMode = NULL;
			}
			LoggerUtil_info(logger, "Novo modo: " << inputModeIndex);
		}
	}

	// Métodos para tratar eventos multimodais.

	void EnhancedInputManager::addMultimodalInputEventListener(
		    IMultimodalInputEventListener* listener) {

		LoggerUtil_info(logger, "addMultimodalInputEventListener("
				"IMultimodalInputEventListener* listener)");
		set<IMultimodalInputEventListener*>::iterator i;
		LockedMultimodalAction* action;

		if (!running) {
			return;
		}

		if (notifying) {
			pthread_mutex_lock(&actMultimodalMutex);
			action = new LockedMultimodalAction;
			action->l      = listener;
			action->isAdd  = true;
			actionsToMultimodalListeners->push_back(action);
			pthread_mutex_unlock(&actMultimodalMutex);

		} else {
			pthread_mutex_lock(&multimodalMutex);
			performMultimodalInputLockedActions();
			multimodalEventListeners->insert(listener);
			pthread_mutex_unlock(&multimodalMutex);
		}
	}

	void EnhancedInputManager::removeMultimodalInputEventListener(
		    IMultimodalInputEventListener* listener) {

		LoggerUtil_info(logger, "removeMultimodalInputEventListener("
				"IMultimodalInputEventListener* listener)");

		set<IMultimodalInputEventListener*>::iterator i;
		LockedMultimodalAction* action;

		if (!running) {
			return;
		}

		if (notifying) {
			pthread_mutex_lock(&actMultimodalMutex);
			action = new LockedMultimodalAction;
			action->l      = listener;
			action->isAdd  = false;
			actionsToMultimodalListeners->push_back(action);
			pthread_mutex_unlock(&actMultimodalMutex);

		} else {
			pthread_mutex_lock(&multimodalMutex);
			performMultimodalInputLockedActions();
			i = multimodalEventListeners->find(listener);
			if (i != multimodalEventListeners->end()) {
				multimodalEventListeners->erase(i);
			}
			pthread_mutex_unlock(&multimodalMutex);
		}
	}

	void EnhancedInputManager::postMultimodalEvent(IMultimodalInputEvent* event) {
		LoggerUtil_info(logger, "postMultimodalEvent("
				"IMultimodalInputEvent* event)");

		DFBUserEvent* dfbEvent = new DFBUserEvent;
		dfbEvent->clazz = (DFBEventClass)DFEC_USER;
//		dfbEvent->type = ???;
		dfbEvent->data = (void*)event;

		DFBMultimodalInputEvent* dfbmie =
				new DFBMultimodalInputEvent((void*)dfbEvent);
		postEvent((IInputEvent*)dfbmie);
	}


	// Esse método foi copiado do InputManager e alterado para dar suporte a
	// eventos do tipo MultimodalInputEvent.
	bool EnhancedInputManager::dispatchMultimodalEvent(
			IMultimodalInputEvent* multimodalInputEvent) {

		LoggerUtil_info(logger, "dispatchMultimodalEvent("
				"IMultimodalInputEvent* multimodalInputEvent)");

		set<IMultimodalInputEventListener*>::iterator i;

		if (!running) {
			return false;
		}

		notifyingMultimodal = true;
		pthread_mutex_lock(&multimodalMutex);

		performMultimodalInputLockedActions();

		if (multimodalEventListeners->empty() || multimodalInputEvent == NULL) {
			pthread_mutex_unlock(&multimodalMutex);
			notifying = false;
			return true;
		}

		// TODO Permitir que a aplicação diga que tipo de evento multimodal
		// deseja receber e notificar aqui apenas os eventos do tipo desejado
		// pela aplicação.
		i = multimodalEventListeners->begin();
		while (i != multimodalEventListeners->end()) {
			if (*i != NULL) {
				if (!(*i)->multimodalUserEventReceived(multimodalInputEvent)) {
					pthread_mutex_unlock(&multimodalMutex);
					notifying = false;
					return false;
				}
			}
			++i;
		}
		pthread_mutex_unlock(&multimodalMutex);
		notifying = false;
		return true;
	}

	void EnhancedInputManager::performMultimodalInputLockedActions() {
		LoggerUtil_info(logger, "performMultimodalInputLockedActions()");

		LockedMultimodalAction* action;
		IMultimodalInputEventListener* listener;

		set<IMultimodalInputEventListener*>::iterator i;
		vector<LockedMultimodalAction*>::iterator j;

		if (!running) {
			return;
		}

		pthread_mutex_lock(&actMultimodalMutex);
		j = actionsToMultimodalListeners->begin();
		while (j != actionsToMultimodalListeners->end()) {
			action = *j;
			listener = action->l;

			if (action->isAdd) {
				multimodalEventListeners->insert(listener);

			} else {
				i = multimodalEventListeners->find(listener);
				if (i != multimodalEventListeners->end()) {
					multimodalEventListeners->erase(i);
				}
			}

			delete action;
			++j;
		}

		actionsToMultimodalListeners->clear();
		pthread_mutex_unlock(&actMultimodalMutex);
	}

	// Estes métodos estão aqui como fachada para os métodos de mesmo nome de
	// CodeMap.
	int EnhancedInputManager::addCode(string codeStr) {
		return codeMap->addCode(codeStr);
	}
		
	bool EnhancedInputManager::removeCode(int code) {
		return codeMap->removeCode(code);
	}

	bool EnhancedInputManager::removeCode(string codeStr) {
		return codeMap->removeCode(codeStr);
	}

	bool EnhancedInputManager::hasCode(int code) {
		return codeMap->hasCode(code);
	}

	bool EnhancedInputManager::hasCode(string codeStr) {
		return codeMap->hasCode(codeStr);
	}

	void EnhancedInputManager::postMultimodalEvent(string xml) {
		LoggerUtil_info(logger, "postMultimodalEvent(string xml)");

		// TODO Criar um EventParser toda vez que um evento chegar? Ver
		// comentário da classe EventParser.
		EventParser* eventParser = new EventParser();

		IMultimodalInputEvent* mmie = eventParser->parse(xml);

		if (mmie != NULL) {
			postMultimodalEvent(mmie);

			// Para permitir que dispositivos gerem eventos simples de tecla.
			// Caso o id do evento seja uma das strings que representam uma
			// tecla, gera um evento normal também.
			// TODO Do jeito que está, o pressionamento está sendo notificado
			// duas vezes, uma no evento multimodal e outra no evento de tecla
			// convencional. Qual seria a melhor foma de fazer isso? Se
			// notificarmos apenas no evento multimodal, aplicações que não são
			// multimodais não irão receber. Se notificarmos apenas como evento
			// normal, aplicações multimodais não poderão fazer uso do
			// recebimento do pressionamento da tecla junto com algum outro
			// dado.
			CodeMap* map = CodeMap::getInstance();
			int key = map->getCode(mmie->getId());
			if (key != CodeMap::KEY_NULL) {
				postEvent(key);
			}
		} else {
			LoggerUtil_info(logger, "Erro de parser. Nada será feito.");
		}


		// TODO Criar um EventParser toda vez que um evento chegar? Ver
		// comentário da classe EventParser.
		delete eventParser;
	}

	void EnhancedInputManager::waitForUnlockCondition() {
		Thread::waitForUnlockCondition();
	}

}
}
}
}
}
}

/**
 * Função que permite a criação de uma instância de IEnhancedInputManager.
 */
extern "C" ::br::ufscar::lince::ginga::core::mmi::IEnhancedInputManager*
		createEnhancedInputManager() {

	return (::br::ufscar::lince::ginga::core::mmi::
			EnhancedInputManager::getInstance());
}

/**
 * Função que permite a criação de uma instância de IEnhancedInputManager.
 */
extern "C" void destroyEnhancedInputManager(
		::br::ufscar::lince::ginga::core::mmi::IEnhancedInputManager* eim) {

	eim->release();
}
