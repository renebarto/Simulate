/*-------------------------------------------------------------------------
Compiler Generator Coco/R,
Copyright (c) 1990, 2004 Hanspeter Moessenboeck, University of Linz
extended by M. Loeberbauer & A. Woess, Univ. of Linz
ported to C++ by Csaba Balazs, University of Szeged
with improvements by Pat Terry, Rhodes University

This program is free software; you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation; either version 2, or (at your option) any 
later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
for more details.

You should have received a copy of the GNU General Public License along 
with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

As an exception, it is allowed to write an extension of Coco/R that is
used as a plugin in non-free software.

If not otherwise stated, any source code generated by Coco/R (other than 
Coco/R itself) does not fall under the GNU General Public License.
-------------------------------------------------------------------------*/

#include "Action.h"
#include "State.h"

namespace Coco {

State::State() {
	this->firstAction = nullptr;
	this->endOf       = nullptr;
	this->ctx         = false;
	this->next        = nullptr;
}

void State::AddAction(Action *act) {
	Action *lasta = nullptr, *a = firstAction;
	while (a != nullptr && act->typ >= a->typ) {lasta = a; a = a->next;}
	// collecting classes at the beginning gives better performance
	act->next = a;
	if (a==firstAction) {
		firstAction = act;
	}
	else {
		lasta->next = act;
	}
}	

void State::DetachAction(Action *act) {
	Action *lasta = nullptr, *a = firstAction;
	while (a != nullptr && a != act) {lasta = a; a = a->next;}
	if (a != nullptr) {
		if (a == firstAction) {
			firstAction = a->next;
		}
		else {
			lasta->next = a->next;
		}
	}
}


void State::MeltWith(State *s) { // copy actions of s to state
	Action *a;
	for (Action *action = s->firstAction; action != nullptr; action = action->next) {
		a = new Action(action->typ, action->sym, action->tc);
		a->AddTargets(action);
		AddAction(a);
	}
}

}; // namespace
