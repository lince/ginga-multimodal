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
 * $Revision: 264 $
 * $Author: mnab $
 * $LastChangedDate: 2008-07-07 18:26:51 +0530 (Mon, 07 Jul 2008) $
 ************************************************************************************/
/************************************************************************
 * FILE DESCR: Trace.cpp
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
#include "../include/TraceFormat.h"
#include "../include/Trace.h"

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
namespace inkmllib{

/**
*Desc - constructor
**/
Trace::Trace()
{
	vectX = new vector<long>();
	vectY = new vector<long>();
	vectF = new vector<int>();
	traceFormatRef = new TraceFormat();
	lastX = lastY = lastF = lastXVel = lastYVel = lastFVel = -1;
}

/**
*Desc - function to parse traceData.
*       It uses the setDataCoordinate method to process 
*       and store co-ordinate values.
**/
void Trace::parseTraceData(char *data, BoundingBox *boundingBox)
{
	if(NULL == data )
		return;

	char *point = strtok(data, ",");
	vector<char *> *vectPoints = new vector<char *>();

	while(point!=NULL)
	{
		vectPoints->push_back(point);
		point = strtok(NULL, ",");
	}

	this->FirstX = 0;
	this->FirstY = 0;
	this->FirstF = 0;
	for(int i = 0; i<(int)vectPoints->size();i++)
	{
		point = vectPoints->at(i);
		// Diogo: De acordo com a especificação InKXML, caracteres que devem ser
		// ignorados são wsp ::= #x20 | #x9 | #xD | #xA. Ou seja, "space",
		// "horizontal tab", "carriage return", "NL line feed, new line".  Por
		// isso, acrescentei o '\t', '\r' e '\n'.
		char *coordinateValue = strtok(point, " \t\r\n");
		int coordinateIndex = 0;
		
		while(coordinateValue!=NULL)
		{
			setDataCoordinate(traceFormatRef->getChannelAtIndex(coordinateIndex),
					coordinateValue, boundingBox);
			// Diogo: mesma obervação acima.
			coordinateValue = strtok(NULL, " \t\r\n");
			
			coordinateIndex++;
		}
	}

	if(vectPoints)
	{
		delete vectPoints;
		vectPoints = NULL;
	}
}
/**
*Desc - function to assign sample point data and 
*       set the bounding box to be used for rendering
*       it parses and expand the single difference encoded 
*       channel data to absolute values
**/
void Trace::setDataCoordinate(CHANNEL coordinate, 
							  char *coordinateValue, 
							  BoundingBox *boundingBox)
{
	long x,y,f;
	switch(coordinate)
	{
	case X:
		
		if(coordinateValue[0]=='\'')
		{
			coordinateValue = GlobalFunction::removeVelocity(coordinateValue);
			this->FirstX = this->lastX;
			x = atol(coordinateValue) + this->lastX;
		}
		else
		{
			x = atol(coordinateValue) + this->FirstX;
		}
		this->lastX = x;

		if(x>boundingBox->maxX)
			boundingBox->maxX = x;

		else if(x<boundingBox->minX)
			boundingBox->minX = x;

		this->vectX->push_back(x);
		break;

	case Y:
		if(coordinateValue[0]=='\'')
		{
			coordinateValue = GlobalFunction::removeVelocity(coordinateValue);
			this->FirstY = this->lastY;
			y = atol(coordinateValue) + this->lastY;
		}
		else
		{
			y = atol(coordinateValue) + this->FirstY;
		}
		this->lastY = y;
		
		if(y>boundingBox->maxY)
			boundingBox->maxY = y;

		else if(y<boundingBox->minY)
			boundingBox->minY = y;

		this->vectY->push_back(y);
		break;

	case F:
		if(coordinateValue[0]=='\'')
		{
			coordinateValue = GlobalFunction::removeVelocity(coordinateValue);
			this->FirstF = this->lastF;
			f = atol(coordinateValue) + this->FirstF;
		}
		else
		{
			f = atol(coordinateValue) + this->FirstF;
		}
		this->lastF = f;

		this->vectF->push_back(atoi(coordinateValue));
		break;
	case UNKNOWN:
		break;
	}
}

}
}
}
}
}
}
}
