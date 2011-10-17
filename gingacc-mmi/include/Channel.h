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
 * $Revision: 242 $
 * $Author: mnab $
 * $LastChangedDate: 2008-07-02 16:28:29 +0530 (Wed, 02 Jul 2008) $
 ************************************************************************************/
/************************************************************************
 * FILE DESCR: Channel.h - models the <channel> element in InkML specification. 
 *      				   Please refer http://www.w3.org/TR/InkML/#channel for more information.
 * AUTHOR:     Birendra Keshari
 * DATE:       16th June 2007
 * Author		Date			Description of change
 * Muthu	    17-June-08      Added constructor with name parameter 
 *                              only and added comments.                                
 * Balaji M N A 18-June-08      Moved class definition methods in .cpp file.
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

#ifndef CHANNEL_H
#define CHANNEL_H

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{
namespace inkmllib{


// enumeration for different units supported for channel data
enum INKML_UNITS {Dev, Inches, Centimeters, 
                  Degrees, Radians, 
                  Seconds, Pounds, Grams};

/**
 * Classe que representa um channel de um InkML, responsável por descrever os
 * dados que podem estar codificados em um trace.
 * Mais informações em http://sourceforge.net/apps/trac/inkmltk/wiki/InkMLLib
 */
class Channel
{
public:
	/// to store the 'name' attribute value. It is a mandatory field.
	char* name; 
	/// to store the 'min' attribute value.
	/// It gives the min range of channel data value.
	long min; 
	/// to store the 'max' attribute value.
	/// It gives the max range of channel data value.
	long max;
	/// If value not given for min and/or max then
	/// the channel value is unbounded in either direction
	/// to store the 'units' attribute value.
	INKML_UNITS units;

public:
	/**
	 * Cria um channel.
	 * @param name Nome do channel.
	 * @param min Limite inferior para os valores do channel
	 * @param max Limite superior para os valores do channel
	 * @param units Unidade em quem os valores do channel estão expressos.
	 */
	Channel(char *name, long min, long max, char *units);

	/**
	 * Cria um channel.
	 * @param name Nome do channel.
	 * @param min Limite inferior para os valores do channel
	 * @param max Limite superior para os valores do channel
	 * @param units Unidade em quem os valores do channel estão expressos.
	 */
	Channel(char *name, long min, long max, INKML_UNITS units);

	/**
	 * Cria um channel.
	 * @param name Nome do channel.
	 */
	Channel(char *name);

	/**
	 * Retorna a unidade em que os valores do channel estão expressos.
	 * @return A unidade
	 */
	INKML_UNITS getUnit(){ return units;}

	/**
	 * Retorna o nome do channel.
	 * @return O nome
	 */
	char* getName(){ return name;}
private:
	INKML_UNITS getUnit(char *units);
};

}
}
}
}
}
}
}

#endif // CHANNEL_H
