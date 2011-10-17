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
 * FILE DESCR: Definitions.h - models the <definitions> element of InkML.
 *			   Please refer http://www.w3.org/TR/InkML/#definitionsElement for more information.
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
- Acréscimo de comentários
*******************************************************************************/
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "TraceFormat.h"
#include "InkSource.h"

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
namespace inkmllib{

/**
 * Definições necessárias para a realização do parser do InkML
 * Mais informações em http://sourceforge.net/apps/trac/inkmltk/wiki/InkMLLib
 */
class Definitions{
private:
	/**
	* Desc - To hold the TraceFormat definitions
	**/
	vector<TraceFormat> *vectTraceFormat;
	/**
	* Desc - To hold the InkSource definitions
	**/
	vector<InkSource> *vectInkSource;

public:
	
	/**
	* Desc - Constructor
	**/
	Definitions()
	{
		vectTraceFormat = new vector<TraceFormat>();
		vectInkSource = new vector<InkSource>();
	}

	/**
	 * Adiciona um novo traceFormat.
	 * @param traceFormat a ser adicionado.
	 */
	void addTraceFormat(TraceFormat *traceFormat)
	{
		vectTraceFormat->push_back(*traceFormat);
	}

	/**
	 * Adiciona um novo inkSource.
	 * @param inkSource a ser adicionado.
	 */
	void addInkSource(InkSource *inkSource)
	{
		this->vectInkSource->push_back(*inkSource);
	}	

	/**
	* Desc - Method returns TraceFormat object with 'id' 
	*        given in the parameter
	**/
	TraceFormat getTraceFormat(char *id)
	{
		TraceFormat traceFormat;
		for(int i = 0; i<(int)this->vectTraceFormat->size(); i++)
		{
			traceFormat = vectTraceFormat->at(i);
			if(strcmp(traceFormat.getID(),id)==0)
				return vectTraceFormat->at(i);
		}

		return NULL;
	}

};

}
}
}
}
}
}
}

#endif // DEFINITION_H
