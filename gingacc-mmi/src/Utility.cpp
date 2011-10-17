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
 * FILE DESCR: Utility.cpp -  This class contains miscellaneous utility methods 
 *						   and enumeration, constant data members
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

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
namespace inkmllib{

//char* GlobalFunction::tinyXmlError;

/**
* Desc - function to remove the '#' character in the reference attribute URI
**/
char* GlobalFunction::checkAndRemoveHash(char *str)
{
	char *newStr;
	if(str[0]=='#')
	{
		newStr = (char *)malloc(sizeof(char)*(strlen(str)-1));
		int i = 1;
		for(; i<(int)strlen(str); i++)
			newStr[i-1]=str[i];
		newStr[i-1] = '\0';
	}
	else
	{
		newStr = (char *)malloc(sizeof(char)*(strlen(str)));
		strcpy(newStr, str);
	}
	return newStr;
}

/**
* Desc - method remove the given prefix <'> to indicate 
*        the single difference and provide the string value of the 
*        co-ordinate differece data 
*        ex: if '12 is given, it returns 12 as string.
**/
char* GlobalFunction::removeVelocity(char *str)
{
	char *newStr;
	if(str[0]=='\'')
	{
		newStr = (char *)malloc(sizeof(char)*(strlen(str)-1));
		int i = 1;
		for(; i<(int)strlen(str); i++)
			newStr[i-1]=str[i];
		newStr[i-1] = '\0';
	}
	else
	{
		newStr = (char *)malloc(sizeof(char)*(strlen(str)));
		strcpy(newStr, str);
	}
	return newStr;
}

/**
* Desc - method takes the enum of the Error message constants and 
*        provide string message
**/
char * GlobalFunction::decodeError(InkMLError errorNo)
{
	switch(errorNo)
	{
	case NoInkRoot:
		return "Error: Document root is not <ink>";
		break;
	case MalFormedXml:
		return "Error: Document is not well formed XML";
		break;
//	case TinyXmlError:
//		return tinyXmlError;
	}

	char* description = new char[100];
	sprintf(description, "Error no %d could not be decoded", errorNo);
	return description;
}

/**
* Desc - function to convert the given string to all lowercase characters
**/
char* GlobalFunction::toLower(char *s)
{
	char *strLower = (char *)malloc(sizeof(char)*strlen(s));
	int i=0;
	for(; i<(int)strlen(s); i++)
	{
		strLower[i] = tolower(s[i]);
	}
	strLower[i] = '\0';
	return strLower;
}

}
}
}
}
}
}
}
