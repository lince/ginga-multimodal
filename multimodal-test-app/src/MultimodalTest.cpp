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

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

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
* @file EnhancedInputManager.h
* @author Caio Viel
* @author Diogo de Carvalho Pedrosa
* @author José Augusto Costa Martins Júnior
* @date 29-01-10
*/

#include "../include/MultimodalTest.h"

#include <mmi/Acceleration.h>

#include <mmi/Ink.h>
using namespace ::br::ufscar::lince::ginga::core::mmi::inkmllib;

//#if HAVE_COMPSUPPORT
#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;

#include <mmi/IEnhancedInputManager.h>
//#else
//#include <mmi/EnhancedInputManager.h>
//#endif


#if HAVE_COMPSUPPORT
#include "system/io/ILocalDeviceManager.h"
#else
#include "../../gingacc-cpp/gingacc-system/include/io/interface/output/dfb/DFBSurface.h"
#include "../../gingacc-cpp/gingacc-system/include/io/interface/output/dfb/DFBWindow.h"
#include "../../gingacc-cpp/gingacc-system/include/io/LocalDeviceManager.h"

#include "../../telemidia-util-cpp/include/Color.h"
#endif


namespace br {
namespace usp {
namespace intermidia {
namespace ginga {
namespace multimodaltest {

	MultimodalTest* MultimodalTest::_instance = NULL;

	MultimodalTest::MultimodalTest() {
		logger = LoggerUtil::getLogger(
				"br.ufscar.lince.ginga.multimodaltest.MultimodalTest");
        LoggerUtil::configure();
		LoggerUtil_info(logger, "Construtor");

		win = NULL;
		// TODO Passar a usar o COMPSURPPORT
		// TODO Pegar tamanho da tela do device
#if HAVE_COMPSUPPORT
		this->surface = ((SurfaceCreator*)(cm->getObject("Surface")))(
				NULL, 720, 480);
#else
		this->surface = new DFBSurface(720, 480);
#endif

		if (this->surface != NULL) {
			this->surface->setCaps(this->surface->getCap("ALPHACHANNEL"));
		}

		// Se não setar a cor, não aparece nada. A cor padrão deve ser preto.
		this->inkColor = new Color("white");
		this->surface->setColor(inkColor);
	}

	MultimodalTest* MultimodalTest::getInstance() {
		if (_instance == NULL) {
			_instance = new MultimodalTest();
		}
		return _instance;
	}

	MultimodalTest::~MultimodalTest() {
		delete this->surface;
		this->surface = NULL;

		if (this->win == NULL) {
			delete this->win;
			this->win = NULL;
		}

	}

	bool MultimodalTest::multimodalUserEventReceived(
			IMultimodalInputEvent* ev) {

		LoggerUtil_info(logger, "multimodalUserEventReceived("
				"IMultimodalInputEvent* ev)");

		LoggerUtil_info(logger, "ID = " << ev->getId());

		map<string, void*>* values = ev->getMultimodalValuesMap();
		for (map<string, void*>::iterator i = values->begin();
				i != values->end(); i++) {
			char* value = (char*)(i->second);
			LoggerUtil_info(logger, "(" << i->first << ", " << value << ")");
		}

		vector<string>* strs = ev->getStrings();
		for (vector<string>::iterator j = strs->begin();
				j != strs->end(); j++) {
			LoggerUtil_info(logger, *j);
		}

		Ink *ink = ev->getInk();

		showInk(ink);

		for (int t = 0; t < ink->vectTrace->size(); t++) {
			Trace trace = ink->vectTrace->at(t);

			cout << "Dados do trace " << t << ": ";

			int limite;
			if (trace.vectX->size() > trace.vectY->size()) {
				limite = trace.vectX->size();
			} else {
				limite = trace.vectY->size();
			}

			for (int i = 0; i < limite; i++) {
				long x = trace.vectX->at(i);
				long y = trace.vectY->at(i);
				cout << x << " " << y << ", ";
			}

			cout << endl;

		}

		vector<File*>* binaries = ev->getBinaries();
		for (vector<File*>::iterator k = binaries->begin();
				k != binaries->end(); k++) {
			LoggerUtil_info(logger, "Nome: " << (*k)->getName() <<
					" Mimetype: " << (*k)->getMimetype());
		}

		Acceleration *accel = ev->getAcceleration();
		LoggerUtil_info(logger, "Aceleração: xValue: " << accel->getXValue() <<
							" yValue: " << accel->getYValue() <<
							" zValue: " << accel->getZValue());

		return true;
	}

	bool MultimodalTest::userEventReceived(IInputEvent* ev) {
		LoggerUtil_info(logger, "userEventReceived(IInputEvent* ev)");

		LoggerUtil_info(logger, "keycode = " << ev->getKeyCode());
		LoggerUtil_info(logger, "type = " << ev->getType());

		return true;
	}

	void MultimodalTest::registerListener() {
		LoggerUtil_info(logger, "registerListener()");

//#if HAVE_COMPSUPPORT
		static IComponentManager* cm = IComponentManager::getCMInstance();
		IEnhancedInputManager* eim  = ((EnhancedInputManagerCreator*)
				(cm->getObject("InputManager")))();
//#else
//		IEnhancedInputManager* eim = IEnhancedInputManager::getInstance();
//#endif

		eim->addMultimodalInputEventListener(this);

		// Pede para receber eventos de pressionamento, mas não de liberação de
		// teclas do controle remoto.
		eim->addInputEventListener(this);
	}

	void MultimodalTest::initializeWindow() {
		int x = 0, y, w, h;
		ILocalDeviceManager* dm = NULL;
#if HAVE_COMPSUPPORT
		dm = ((LocalDeviceManagerCreator*)(
				cm->getObject("LocalDeviceManager")))();
#else
		dm = LocalDeviceManager::getInstance();
#endif

		if (dm != NULL) {
			x = 0;
			y = 0;
			w = (int)(dm->getDeviceWidth());
			h = (int)(dm->getDeviceHeight());
		}

#if HAVE_COMPSUPPORT
		win = ((WindowCreator*)(cm->getObject("Window")))(x, y, w, h);
#else
		win = new DFBWindow(x, y, w, h);
#endif

		win->setCaps(win->getCap("ALPHACHANNEL"));
		win->draw();
	}

	void MultimodalTest::showInk(Ink *ink) {
		LoggerUtil_info(logger, "showInk(Ink *ink)");

		// TODO Mostrar dados de ink em vez desse retângulo.
//		this->surface->fillRectangle(10, 10, 50, 50);

		for (int t = 0; t < ink->vectTrace->size(); t++) {
			Trace trace = ink->vectTrace->at(t);

			long xOld;
			long yOld;

			int limite;
			if (trace.vectX->size() > trace.vectY->size()) {
				limite = trace.vectX->size();
			} else {
				limite = trace.vectY->size();
			}

			if (limite > 0) {
				xOld = trace.vectX->at(0);
				yOld = trace.vectY->at(0);
			}

			for (int i = 1; i < limite; i++) {
				long x = trace.vectX->at(i);
				long y = trace.vectY->at(i);
				this->surface->drawLine(xOld, yOld, x, y);
				xOld = x;
				yOld = y;
			}

			cout << endl;

		}

		if (win == NULL) {
			initializeWindow();
		}

		if (surface->setParent((void*)win)) {
			win->renderFrom(surface);
		}

		win->show();
		win->raiseToTop();
	}

}
}
}
}
}
