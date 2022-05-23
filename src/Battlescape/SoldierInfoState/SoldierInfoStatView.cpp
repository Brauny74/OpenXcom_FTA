/*
 * Copyright 2010-2020 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SoldierInfoStatView.h"
#include "../../Interface/Bar.h"
#include "../../Interface/TextButton.h"
#include "../../Interface/Text.h"
#include <string>

namespace OpenXcom
{

SoldierInfoStatView::SoldierInfoStatView(Text *txt, Text *num, Bar *bar)
{
	_bar = bar;
	_txt = txt;
	_num = num;
}

void SoldierInfoStatView::setVisible(bool visible){
	_bar->setVisible(visible);
	_num->setVisible(visible);
	_txt->setVisible(visible);
}

void SoldierInfoStatView::setValue(std::string numText, double barMaxValue, double barValue, double barValue2)
{
	_num->setText(numText);
	_bar->setMax(barMaxValue);
	_bar->setValue(barValue);
	_bar->setValue2(barValue2);
}

}
