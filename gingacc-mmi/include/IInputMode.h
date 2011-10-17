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
* @file IInputMode.h
* @author Caio Viel
* @date 29-01-10
*/
#ifndef IINPUTMODE_H_
#define IINPUTMODE_H_

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

/**
 * Interface que representa um modo de entrada de eventos. Pode representar um
 * redirecionador ou um modificador de eventos.
 */
class IInputMode {

	public:
		/// Representa Modos de entrada redirecionadores de eventos.
		static const int REDIRECTER = 0;

		/// Represente Modos de entrada modificadores de eventos.
		static const int MODIFIER = 1;

	public:
		virtual ~IInputMode(){};

		/**
		 * Método chamado quando o modo de entrada é ativado.
		 */
		virtual void startingInputMode()=0;

		/**
		 * Método chamado quando o modo de entrada é desativado.
		 */
		virtual void exitingInputMode()=0;
		
		/**
		 * Método chamado quando o modo de entrada é adicionado aos modos
		 * existentes.
		 */
		virtual void initializeInputMode()=0;

		/**
		 * Retorna um constante inteira que representa o tipo de modo de
		 * entrada.
		 * @return REDIRECTER se for um modo redirecionador; MODIFIER se for um
		 * modo modificador.
		 */
		virtual int getInputModeType()=0;

};

}
}
}
}
}
}

#endif /*IINPUTMODE_H_*/
