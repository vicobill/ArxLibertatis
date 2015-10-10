/*
 * Copyright 2015 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ARX_GUI_WIDGET_HORIZONTALPANELWIDGET_H
#define ARX_GUI_WIDGET_HORIZONTALPANELWIDGET_H

#include "gui/widget/Widget.h"

class HorizontalPanelWidget : public Widget {
	
public:
	HorizontalPanelWidget();
	virtual ~HorizontalPanelWidget();
	
	void Move(const Vec2i & offset);
	void AddElement(Widget * widget);
	void AddElementNoCenterIn(Widget * widget);
	
	void Update(int time);
	void Render();
	bool OnMouseClick() { return false; }
	Widget * OnShortCut();
	void RenderMouseOver() { }
	Widget * IsMouseOver(const Vec2s & mousePos) const;
	Widget * GetZoneWithID(int zoneId);
	
private:
	std::vector<Widget *>	vElement;
};

#endif // ARX_GUI_WIDGET_HORIZONTALPANELWIDGET_H
