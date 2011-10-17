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
* @file EventParser.cpp
* @author Diogo de Carvalho Pedrosa
* @date 03-09-10
*/

#include "../include/EventParser.h"
#include "../include/MultimodalInputEvent.h"
#include "../include/Acceleration.h"
#include "../include/File.h"
#include "../include/Ink.h"
#include "../include/InkMLParser.h"
#include "../include/Utility.h"

using namespace ::br::ufscar::lince::ginga::core::mmi::inkmllib;

//#include "cm/IComponentManager.h"
//using namespace ::br::pucrio::telemidia::ginga::core::cm;

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/util/Base64.hpp>	// Para decode (decodificar arquivos binários
									// contidos nos eventos multimodais)
#include <xercesc/util/XercesDefs.hpp>	// Para XMLByte
XERCES_CPP_NAMESPACE_USE

#include <iostream>
#include <fstream>	// Para usar ofstream para escrever o arquivo recebido no
					// evento.
using namespace std;

// Para usar atoi
#include <cstdlib>

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

	EventParser::EventParser() {
		logger = LoggerUtil::getLogger(
				"br.ufscar.lince.ginga.core.mmi.EventParser");
		LoggerUtil_info(logger, "Construtor");

		// TODO Isso deve ser chamado apenas uma vez, por isso não pode ficar
		// aqui. Na classe CMLParsing, fica no construtor.
		XMLPlatformUtils::Initialize();
	}

	EventParser::~EventParser() {
		LoggerUtil_info(logger, "Destrutor");

		// TODO Isso deve ser chamado apenas uma vez, por isso não pode ficar
		// aqui. Na classe CMLParsing, fica no destrutor.
		XMLPlatformUtils::Terminate();
	}

	/**
	 *     enum NodeType {
        ELEMENT_NODE                = 1,
        ATTRIBUTE_NODE              = 2,
        TEXT_NODE                   = 3,
        CDATA_SECTION_NODE          = 4,
        ENTITY_REFERENCE_NODE       = 5,
        ENTITY_NODE                 = 6,
        PROCESSING_INSTRUCTION_NODE = 7,
        COMMENT_NODE                = 8,
        DOCUMENT_NODE               = 9,
        DOCUMENT_TYPE_NODE          = 10,
        DOCUMENT_FRAGMENT_NODE      = 11,
        NOTATION_NODE               = 12
    };
	 */
	IMultimodalInputEvent* EventParser::parse(string xml) {
		LoggerUtil_info(logger, "parse(string xml)");

		bool bFailed = false;

		// create new parser instance.
		XercesDOMParser *parser = new XercesDOMParser();
		if (!parser) {
			LoggerUtil_info(logger, "não foi possível criar um parser");
			return NULL;
		} else {
			// TODO Não sei que tipo de validação é possível ser feita nem
			// entendo bem essas chamadas comentadas abaixo.

//			parser->setValidationScheme(XercesDOMParser::Val_Auto);
//			parser->setDoNamespaces(false);
//			parser->setDoSchema(false);

			// skip this if you haven't written your own error
			// reporter class.
//			DOMTreeErrorReporter *errReporter = new DOMTreeErrorReporter();
//			parser->setErrorHandler(errReporter);
//			parser->setCreateEntityReferenceNodes(false);

			//parser->setToCreateXMLDeclTypeNode(true);
			try {
				//checking if source is xml or uri
				if (xml.find("<") != std::string::npos) {
					MemBufInputSource xmlSource(
							(XMLByte*)(xml.c_str()),
							xml.length(),
							XMLString::transcode("xmlContent"));

					parser->parse(xmlSource);

				} else {
					LocalFileInputSource source(
							XMLString::transcode(xml.c_str()));

					parser->parse(source);
				}

				bFailed = parser->getErrorCount() != 0;
				if (bFailed) {
//					cerr << "Parsing " << xml <<std::endl;
					cerr << " error count: ";
					cerr << parser->getErrorCount() << std::endl;
				}

			} catch (const DOMException& e) {
				cerr << "DOM Exception parsing ";
				cerr << xml;
				cerr << " reports: ";

				// was message provided?
				if (e.msg) {
					// yes: display it as ascii.
					char *strMsg = XMLString::transcode(e.msg);
					cerr << strMsg << std::endl;
					XMLString::release(&strMsg);

				} else {
					// no: just display the error code.
					cerr << e.code << std::endl;
				}

				bFailed = true;

			} catch (const XMLException& e) {
				cerr << "XML Exception parsing ";
				cerr << xml;
				cerr << " reports: ";
				cerr << e.getMessage() << std::endl;
				bFailed = true;

			} catch (const SAXException& e) {
				cerr << "SAX Exception parsing ";
				cerr << xml;
				cerr << " reports: ";
				cerr << e.getMessage() << std::endl;
				bFailed = true;

			} catch (...) {
				cerr << "An exception parsing ";
				cerr << xml << std::endl;
				bFailed = true;
			}

			// did the input document parse okay?
			if (!bFailed) {
				DOMDocument *pDoc = parser->getDocument();
				DOMElement* elementRoot = pDoc->getDocumentElement();

				if (!elementRoot) {
					LoggerUtil_info(logger, "empty XML document");
//					throw(std::runtime_error("empty XML document"));
				}

				this->parseRoot(elementRoot);

				return this->multimodalEvent;
			}
		}

		return NULL;
	}

	void EventParser::parseRoot(DOMElement* e) {
		LoggerUtil_info(logger, "parseRoot");

		if (XMLString::equals(e->getTagName(),
				XMLString::transcode("multimodal"))) {

			const XMLCh* id = e->getAttribute(XMLString::transcode("id"));
//			LoggerUtil_info(logger, XMLString::transcode(id));
			this->multimodalEvent =
					new MultimodalInputEvent(XMLString::transcode(id));
		}

		// Acessa os filhos do elemento raíz e sua quantidade.
		DOMNodeList* children = e->getChildNodes();
		const XMLSize_t nodeCount = children->getLength();

		// Procura os elementos head e body, dentre os filhos do elemento raiz.
		for (XMLSize_t i = 0; i < nodeCount; i++) {
			DOMNode* currentNode = children->item(i);

			if (currentNode->getNodeType() == DOMNode::ELEMENT_NODE) {
				DOMElement* currentElement
					= dynamic_cast<xercesc::DOMElement*>(currentNode);

				if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("head"))) {
					this->parseHead(currentElement);
				} else if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("body"))) {
					this->parseBody(currentElement);
				} else {
					LoggerUtil_info(logger, "ERRO! Tag inesperada: " <<
						XMLString::transcode(currentElement->getTagName()));
				}
			}
		}
	}

	void EventParser::parseHead(DOMElement* e) {
		LoggerUtil_info(logger, "parseHead");

		DOMNodeList* children = e->getChildNodes();
		const XMLSize_t nodeCount = children->getLength();

		for (XMLSize_t i = 0; i < nodeCount; i++) {
			DOMNode* currentNode = children->item(i);

//			LoggerUtil_info(logger, currentNode->getNodeType());

			if (currentNode->getNodeType() == DOMNode::ELEMENT_NODE) {
				DOMElement* currentElement
					= dynamic_cast<xercesc::DOMElement*>(currentNode);

				if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("device"))) {

					const XMLCh* attribute = currentElement->getAttribute(
						XMLString::transcode("id"));
//					LoggerUtil_info(logger, XMLString::transcode(attribute));
					this->multimodalEvent->addValue("device_id",
							XMLString::transcode(attribute));

					attribute = currentElement->getAttribute(
						XMLString::transcode("model"));
//					LoggerUtil_info(logger, XMLString::transcode(attribute));
					this->multimodalEvent->addValue("device_model",
							XMLString::transcode(attribute));

				} else if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("user"))) {

					const XMLCh* attribute = currentElement->getAttribute(
						XMLString::transcode("id"));
//					LoggerUtil_info(logger, XMLString::transcode(attribute));
					this->multimodalEvent->addValue("user_id",
							XMLString::transcode(attribute));

				} else if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("timestamp"))) {

					const XMLCh* attribute = currentElement->getAttribute(
						XMLString::transcode("begin"));
//					LoggerUtil_info(logger, XMLString::transcode(attribute));
					this->multimodalEvent->addValue("time_begin",
							XMLString::transcode(attribute));

					attribute = currentElement->getAttribute(
						XMLString::transcode("end"));
//					LoggerUtil_info(logger, XMLString::transcode(attribute));
					this->multimodalEvent->addValue("time_end",
							XMLString::transcode(attribute));

				} else {
					LoggerUtil_info(logger, "ERRO! Tag inesperada: " <<
						XMLString::transcode(currentElement->getTagName()));
				}
//			} else {
//				LoggerUtil_info(logger, "Não é elemento");
			}
		}
	}

	void EventParser::parseBody(DOMElement* e) {
		LoggerUtil_info(logger, "parseBody");

		DOMNodeList* children = e->getChildNodes();
		const XMLSize_t nodeCount = children->getLength();

		for (XMLSize_t i = 0; i < nodeCount; i++) {
			DOMNode* currentNode = children->item(i);

//			LoggerUtil_info(logger, currentNode->getNodeType());

			if (currentNode->getNodeType() == DOMNode::ELEMENT_NODE) {
				DOMElement* currentElement
					= dynamic_cast<xercesc::DOMElement*>(currentNode);

				if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("value"))) {

					const XMLCh* key = currentElement->getAttribute(
						XMLString::transcode("id"));

					DOMNode* node = currentElement->getFirstChild();

					if (node && node->getNodeType() == DOMNode::TEXT_NODE) {
						DOMText* text = dynamic_cast<xercesc::DOMText*>(node);
						this->multimodalEvent->addValue(
								XMLString::transcode(key),
								XMLString::transcode(text->getData()));
					}

				} else if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("text"))) {

					DOMNode* node = currentElement->getFirstChild();

					if (node && node->getNodeType() == DOMNode::TEXT_NODE) {
						DOMText* text = dynamic_cast<xercesc::DOMText*>(node);
						this->multimodalEvent->addString(
								XMLString::transcode(text->getData()));
					}

				} else if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("inkml:ink"))) {
					// TODO e se o xml vier com um prefixo de namespace
					// diferente???

					InkMLParser *inkMLParser = new InkMLParser();
					InkMLError inkMLError;
					Ink* ink = inkMLParser->parse(currentElement, &inkMLError);

					this->multimodalEvent->setInk(ink);

				} else if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("voice"))) {

					DOMNode* node = currentElement->getFirstChild();

					if (node && node->getNodeType() == DOMNode::TEXT_NODE) {
						DOMText* text = dynamic_cast<xercesc::DOMText*>(node);
						this->multimodalEvent->addValue("voice",
								XMLString::transcode(text->getData()));
					}

				} else if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("accel"))) {

					const XMLCh* attX = currentElement->getAttribute(
						XMLString::transcode("xValue"));
					const XMLCh* attY = currentElement->getAttribute(
						XMLString::transcode("yValue"));
					const XMLCh* attZ = currentElement->getAttribute(
						XMLString::transcode("zValue"));

					Acceleration* accel = new Acceleration(
							atoi(XMLString::transcode(attX)),
							atoi(XMLString::transcode(attY)),
							atoi(XMLString::transcode(attZ)));

					this->multimodalEvent->setAcceleration(accel);


				} else if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("binary"))) {

					const XMLCh* filename = currentElement->getAttribute(
						XMLString::transcode("filename"));
					const XMLCh* mimetype = currentElement->getAttribute(
						XMLString::transcode("mimetype"));

					File* file = new File(XMLString::transcode(filename),
							XMLString::transcode(mimetype));

					this->multimodalEvent->addFile(file);

					DOMNode* node = currentElement->getFirstChild();

					if (node && node->getNodeType() == DOMNode::TEXT_NODE) {
						DOMText* text = dynamic_cast<xercesc::DOMText*>(node);

//						LoggerUtil_info(logger, "text->getData():");
//						LoggerUtil_info(logger, XMLString::transcode(text->getData()));

						XMLByte* inputData = (XMLByte*)
								XMLString::transcode(text->getData());
						unsigned int decodedLength;
						XMLByte* decodedPayload = Base64::decode(inputData,
								&decodedLength);
						char* decodedPayloadChar = (char*)decodedPayload;

					    ofstream myFile (XMLString::transcode(filename),
					    		ios::out | ios::binary);
					    myFile.write (decodedPayloadChar, decodedLength);
					}

				} else {
					LoggerUtil_info(logger, "ERRO! Tag inesperada: " <<
						XMLString::transcode(currentElement->getTagName()));
				}
//			} else {
//				LoggerUtil_info(logger, "Não é elemento");
			}
		}
	}
}
}
}
}
}
}


//extern "C" ::br::ufscar::lince::ginga::core::mmi::IEnhancedInputManager*
//		createEnhancedInputManager() {
//
//	return (::br::ufscar::lince::ginga::core::mmi::
//			EnhancedInputManager::getInstance());
//}
//
//extern "C" void destroyEnhancedInputManager(
//		::br::ufscar::lince::ginga::core::mmi::IEnhancedInputManager* eim) {
//
//	eim->release();
//}
