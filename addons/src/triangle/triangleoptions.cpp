/*
 *  Copyright (C) 2023 Nick Egorrov, nicegorov@yandex.ru
 *
 *  This file is part of GCodeWorkShop.
 *
 *  GCodeWorkShop is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QSettings>    // for QSettings
#include <QVariant>     // for QVariant

#include "triangleoptions.h"


#define CFG_KEY_ANGLE_A_IN      "AngleAIn"
#define CFG_KEY_ANGLE_A_VALUE   "AngleAValue"
#define CFG_KEY_ANGLE_B_IN      "AngleBIn"
#define CFG_KEY_ANGLE_B_VALUE   "AngleBValue"
#define CFG_KEY_ANGLE_C_IN      "AngleCIn"
#define CFG_KEY_ANGLE_C_VALUE   "AngleCValue"
#define CFG_KEY_SIDE_A_IN       "SideAIn"
#define CFG_KEY_SIDE_A_VALUE    "SideAValue"
#define CFG_KEY_SIDE_B_IN       "SideBIn"
#define CFG_KEY_SIDE_B_VALUE    "SideBValue"
#define CFG_KEY_SIDE_C_IN       "SideCIn"
#define CFG_KEY_SIDE_C_VALUE    "SideCValue"
#define CFG_KEY_RIGHT_TRIANGLE  "RightTriangle"


TriangleOptions::TriangleOptions() :
	rightTriangle{true},
	angleA{false, 0.0},
	angleB{false, 0.0},
	angleC{false, 0.0},
	sideA{false, 0.0},
	sideB{true, 30.0},
	sideC{true, 40.0}
{
}

void TriangleOptions::load(QSettings* settings)
{
	angleA.in = settings->value(CFG_KEY_ANGLE_A_IN, angleA.in).toBool();
	angleA.value = settings->value(CFG_KEY_ANGLE_A_VALUE, angleA.value).toDouble();
	angleB.in = settings->value(CFG_KEY_ANGLE_B_IN, angleB.in).toBool();
	angleB.value = settings->value(CFG_KEY_ANGLE_B_VALUE, angleB.value).toDouble();
	angleC.in = settings->value(CFG_KEY_ANGLE_C_IN, angleC.in).toBool();
	angleC.value = settings->value(CFG_KEY_ANGLE_C_VALUE, angleC.value).toDouble();
	sideA.in = settings->value(CFG_KEY_SIDE_A_IN, sideA.in).toBool();
	sideA.value = settings->value(CFG_KEY_SIDE_A_VALUE, sideA.value).toDouble();
	sideB.in = settings->value(CFG_KEY_SIDE_B_IN, sideB.in).toBool();
	sideB.value = settings->value(CFG_KEY_SIDE_B_VALUE, sideB.value).toDouble();
	sideC.in = settings->value(CFG_KEY_SIDE_B_IN, sideC.in).toBool();
	sideC.value = settings->value(CFG_KEY_SIDE_B_VALUE, sideC.value).toDouble();
	rightTriangle = settings->value(CFG_KEY_RIGHT_TRIANGLE, rightTriangle).toBool();
}

void TriangleOptions::save(QSettings* settings)
{
	settings->setValue(CFG_KEY_ANGLE_A_IN, angleA.in);
	settings->setValue(CFG_KEY_ANGLE_A_VALUE, angleA.value);
	settings->setValue(CFG_KEY_ANGLE_B_IN, angleB.in);
	settings->setValue(CFG_KEY_ANGLE_B_VALUE, angleB.value);
	settings->setValue(CFG_KEY_ANGLE_C_IN, angleC.in);
	settings->setValue(CFG_KEY_ANGLE_C_VALUE, angleC.value);
	settings->setValue(CFG_KEY_SIDE_A_IN, sideA.in);
	settings->setValue(CFG_KEY_SIDE_A_VALUE, sideA.value);
	settings->setValue(CFG_KEY_SIDE_B_IN, sideB.in);
	settings->setValue(CFG_KEY_SIDE_B_VALUE, sideB.value);
	settings->setValue(CFG_KEY_SIDE_B_IN, sideC.in);
	settings->setValue(CFG_KEY_SIDE_B_VALUE, sideC.value);
	settings->setValue(CFG_KEY_RIGHT_TRIANGLE, rightTriangle);
}
