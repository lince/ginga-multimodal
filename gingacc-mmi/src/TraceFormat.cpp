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
 * $Revision: 280 $
 * $Author: selvarmu $
 * $LastChangedDate: 2008-08-05 09:14:15 +0530 (Tue, 05 Aug 2008) $
 ************************************************************************************/
/************************************************************************
 * FILE DESCR: TraceFormat.cpp
 * AUTHOR:     Birendra
 * DATE:       24th July 2007
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
- Ajuste nos caminhos dos includes;
- Acréscimo dos namespaces.
*******************************************************************************/
#include "../include/Utility.h"
#include "../include/TraceFormat.h"

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
namespace inkmllib{

// constructors
TraceFormat::TraceFormat()
{
	vectChannel = new vector<Channel>();
	vectChannelOrder = new vector<CHANNEL>();
	channelFPresent = false;
}

TraceFormat::TraceFormat(char *id)
{
	vectChannel = new vector<Channel>();
	vectChannelOrder = new vector<CHANNEL>();
	channelFPresent = false;
	this->id = id;
}

void TraceFormat::addChannelOrder(char *c)
{
	if(NULL == c)
		return;

	char *lower = GlobalFunction::toLower(c);
	CHANNEL ch = UNKNOWN;

	if(strcmp(lower, "x")==0)
		ch = X;
	else if(strcmp(lower, "y")==0)
		ch = Y;
	else if(strcmp(lower, "f")==0)
		ch = F;

	this->vectChannelOrder->push_back(ch);
}

/**
* Returns CHANNEL at a specified index.
* This can be used to determine the order of channel in trace format.
* @param index The index.
* @return The CHANNEL.
*/
CHANNEL TraceFormat::getChannelAtIndex(int index)
{
	if(vectChannelOrder->size()>=2)
	{
		CHANNEL ch = this->vectChannelOrder->at(index);
		return ch;
	}
	//Default order
	if(index == 0)
		return X;
	else if(index > 1)
		return UNKNOWN;
	return Y;
}

// create data object of the constituent channels from the input XML data
/*void TraceFormat::initializeFromXMLNode(TiXmlNode *node)
{
	if(!node)
		return;

	TiXmlNode *child;
	for(child = node->FirstChild();child; child=child->NextSibling())
	{
		if(strcmp(child->Value(), "channel")==0)
		{
			TiXmlElement *childElement = child->ToElement();
			//Find name of the channel
			const char *name = childElement->Attribute("name");
			int min = -1, max = -1;
			childElement->QueryIntAttribute("min", &min);
			childElement->QueryIntAttribute("max", &max);
			const char *units = childElement->Attribute("units");
			if(!units)
				units = "";

			Channel c((char *)name, min, max, (char *)units);
			vectChannel->push_back(c);
			this->addChannelOrder((char *)name);
		}
	}
}
*/
}
}
}
}
}
}
}
