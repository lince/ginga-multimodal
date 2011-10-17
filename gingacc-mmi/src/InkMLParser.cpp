/*************************************************************************
 * Copyright (c) 2008 Hewlett-Packard Development Company, L.P.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **************************************************************************/
/************************************************************************
 * SVN MACROS
 *
 * $Revision: 245 $
 * $Author: mnab $
 * $LastChangedDate: 2008-07-04 15:49:12 +0530 (Fri, 04 Jul 2008) $
 ************************************************************************************/
/************************************************************************
 * FILE DESCR: InkMLParser.cpp 
 * CONTENTS:   This class uses tinyxml to parse InkML and create corresponding Ink object
 *			   with all absolute context of each trace/trace group resolved. This is used
 *			   by the plugin to render the traces. This InkML parser only processes those
 *             elements that are required for rendering eg. * contribute to the context of
 *             trace/trace group and hence other elements might not even appear in the Ink object.
 * AUTHOR:     Birendra
 * DATE:       16th June 2007
 * CHANGE HISTORY:
 * Author		Date			Description of change
 * Balaji M N A 18-June-08      Moved class definition methods into .cpp file 
 *                              from .h file.
 ************************************************************************/
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
/*******************************************************************************
As seguintes modificações foram feitas em cima do código original:
- Troca dos código do TinyXML por código do Xerces
- Acrécimo do logger
- Ajuste nos caminhos dos includes;
- Acréscimo dos namespaces.
*******************************************************************************/
#include "../include/InkMLParser.h"
#include <vector>

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
namespace inkmllib{

	InkMLParser::InkMLParser() {
		logger = LoggerUtil::getLogger(
				"br.ufscar.lince.ginga.core.mmi.inkmllib.InkMLParser");
		LoggerUtil_info(logger, "Construtor");
	}

	Ink* InkMLParser::parse(DOMElement *inkMLData, InkMLError *retCode) {

		LoggerUtil_info(logger, "void InkMLParser::parse(DOMElement *inkMLData,"
				" InkMLError *retCode)");

		if(!inkMLData) {
			return NULL;
		}

		Ink *objInk = new Ink();

		// Verifica se o DOMElement recebido é um ink.
		if (XMLString::equals(inkMLData->getTagName(),
				XMLString::transcode("inkml:ink"))) {
			// TODO e se o xml vier com um prefixo de namespace
			// diferente???

			DOMNodeList* children = inkMLData->getChildNodes();
			const XMLSize_t nodeCount = children->getLength();

			for (XMLSize_t i = 0; i < nodeCount; i++) {
				DOMNode* currentNode = children->item(i);

				if (currentNode->getNodeType() == DOMNode::ELEMENT_NODE) {
					DOMElement* currentElement
						= dynamic_cast<xercesc::DOMElement*>(currentNode);

					// Dentro de uma tag ink podem haver 4 tags diferentes:
					// definitions, context, trace ou traceGroup. Por enquanto,
					// só testamos o parser da tag trace.
					if (XMLString::equals(currentElement->getTagName(),
							XMLString::transcode("inkml:definitions"))) {

						parseDefinitions(currentElement, objInk);

					} else if (XMLString::equals(currentElement->getTagName(),
							XMLString::transcode("inkml:context"))) {

						LoggerUtil_info(logger, "context");

						const XMLCh* ref = currentElement->getAttribute(
							XMLString::transcode("inkml:traceFormatRef"));

						if (ref) {
							char *attrNew = GlobalFunction::checkAndRemoveHash(
								XMLString::transcode(ref));
							TraceFormat traceFormat = objInk->getDefinitions()->
									getTraceFormat(attrNew);

							if(&traceFormat != NULL) {
								objInk->currentContext->traceFormatRef =
										&traceFormat;
							}
						}
					} else if (XMLString::equals(currentElement->getTagName(),
								XMLString::transcode("inkml:trace"))) {
						insertTrace(currentElement, objInk);

					} else if (XMLString::equals(currentElement->getTagName(),
								XMLString::transcode("inkml:traceGroup"))) {

						DOMNodeList* children = currentElement->getChildNodes();
						const XMLSize_t nodeCount = children->getLength();

						for (XMLSize_t i = 0; i < nodeCount; i++) {
							DOMNode* currentNode = children->item(i);

							if (currentNode->getNodeType() ==
									DOMNode::ELEMENT_NODE) {

								DOMElement* currentElement = dynamic_cast
										<xercesc::DOMElement*>(currentNode);

								if (XMLString::equals(currentElement->
										getTagName(),
										XMLString::transcode("inkml:trace"))) {
									insertTrace(currentElement, objInk);
								}
							}
						}
					} else {
						LoggerUtil_info(logger, "ERRO! Tag inesperada: " <<
							XMLString::transcode(currentElement->getTagName()));
					}
				}
			}
		} else {
			*retCode = NoInkRoot;
			return objInk;
		}

		*retCode = NoError;//Success
		return objInk;
	}

	void InkMLParser::parseDefinitions(DOMElement *e, Ink *objInk) {
		LoggerUtil_info(logger, "parseDefinitions(DOMElement *e, Ink *objInk)");

		Definitions *objDefinitions = new Definitions();

		DOMNodeList* children = e->getChildNodes();
		const XMLSize_t nodeCount = children->getLength();

		for (XMLSize_t i = 0; i < nodeCount; i++) {
			DOMNode* currentNode = children->item(i);

			if (currentNode->getNodeType() == DOMNode::ELEMENT_NODE) {

				DOMElement* currentElement =
						dynamic_cast<xercesc::DOMElement*>(currentNode);

				if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("inkml:traceFormat"))) {

					TraceFormat *traceFormat = new TraceFormat();

					const XMLCh* id = currentElement->getAttribute(
							XMLString::transcode("id"));
					traceFormat->id = XMLString::transcode(id);
//								traceFormat->initializeFromXMLNode(defChild);
					objDefinitions->addTraceFormat(traceFormat);

				} else if (XMLString::equals(currentElement->getTagName(),
						XMLString::transcode("inkml:inkSource"))) {

					InkSource *objInkSource = new InkSource();

					DOMNodeList* children = currentElement->getChildNodes();
					const XMLSize_t nodeCount = children->getLength();

					for (XMLSize_t i = 0; i < nodeCount; i++) {
						DOMNode* currentNode = children->item(i);

						if (currentNode->getNodeType() ==
								DOMNode::ELEMENT_NODE) {

							DOMElement* currentElement = dynamic_cast
									<xercesc::DOMElement*>(currentNode);

							if (XMLString::equals(currentElement->getTagName(),
									XMLString::transcode("inkml:traceFormat"))) {

								TraceFormat *traceFormat = new TraceFormat();
								const XMLCh* id = currentElement->getAttribute(
										XMLString::transcode("id"));
								traceFormat->id = XMLString::transcode(id);
//								traceFormat->initializeFromXMLNode(defChild);
								objDefinitions->addTraceFormat(traceFormat);
							}
						}
					}

					objDefinitions->addInkSource(objInkSource);
				}

				objInk->addDefinitions(objDefinitions);
			}
		}
	}

	void InkMLParser::insertTrace(DOMElement *e, Ink *objInk) {
		LoggerUtil_info(logger, "insertTrace(DOMElement *e, Ink *objInk)");

		if (NULL == e && NULL == objInk) {
			return;
		}

		Trace trace;

		if(objInk->currentContext->traceFormatRef != NULL) {
			trace.setTraceFormatRef(objInk->currentContext->traceFormatRef);
		}

		DOMNodeList* children = e->getChildNodes();
		const XMLSize_t nodeCount = children->getLength();

		for (XMLSize_t i = 0; i < nodeCount; i++) {
			DOMNode* currentNode = children->item(i);
			if (currentNode->getNodeType() == DOMNode::TEXT_NODE) {
				DOMText* text = dynamic_cast<xercesc::DOMText*>
					(currentNode);
				char* traceChar = XMLString::transcode(text->getData());
				trace.parseTraceData(traceChar, objInk->boundingBox);
				objInk->addTrace(trace);
			}
		}
	}

}
}
}
}
}
}
}
