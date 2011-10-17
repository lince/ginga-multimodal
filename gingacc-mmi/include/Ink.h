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
 * FILE DESCR: Ink.h - models the <ink> element of InkML.
 *			   Please refer http://www.w3.org/TR/InkML/#inkElement for more information.
 * CONTENTS:   
 * AUTHOR:     Birendra Keshari
 * DATE:       24th July 2007
 * CHANGE HISTORY:
 * Author		Date			Description of change
 * Muthu	   17-June-08		Added comment statements
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
- Acréscimo dos namespaces.
*******************************************************************************/
#ifndef INK_H
#define INK_H

#include "Definitions.h"
#include "Trace.h"
#include "Context.h"

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
namespace inkmllib{

/**
 * Classe que representa dados de tinta contidos em um MultimodalInputEvent.
 * Mais informações em http://sourceforge.net/apps/trac/inkmltk/wiki/InkMLLib
 */
class Ink{
	/** 
	* Desc - To store definition state of this ink document
	**/
	Definitions *definitions;
public:
	/** 
	* Desc - To capture the boundary of the rectangle inside whice the 
	*        traces of this ink document would be rendered.
	**/
	BoundingBox *boundingBox;
	/** 
	* Desc - To hold all traces of this ink document.
	**/
	vector<Trace> *vectTrace;
	/** 
	* Desc - handle to the current context of this ink document.
	**/
	Context *currentContext;

	/** 
	* Desc - constructor to create an ink document.
	**/
	Ink();

	/** 
	* Desc - assign the definitions state of this ink document.
	**/	
	void addDefinitions(Definitions *definitions)
	{
		this->definitions = definitions;
	}

	/** 
	* Desc - function to get the definitions state of this ink document.
	**/	
	Definitions * getDefinitions()
	{
		return this->definitions;
	}

	/** 
	* Desc - function to add a Trace to this ink document.
	**/		
	void addTrace(Trace trace)
	{
		vectTrace->push_back(trace);
	}

	/** 
	* Desc - function to normalize trace sample point data to fit within 
	*        the given boundary box for rendering.
	**/	
	void normalizeTracePoint(long width, long height);
};

}
}
}
}
}
}
}

#endif // INK_H
