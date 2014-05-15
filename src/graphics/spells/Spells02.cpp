/*
 * Copyright 2011-2012 Arx Libertatis Team (see the AUTHORS file)
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
/* Based on:
===========================================================================
ARX FATALIS GPL Source Code
Copyright (C) 1999-2010 Arkane Studios SA, a ZeniMax Media company.

This file is part of the Arx Fatalis GPL Source Code ('Arx Fatalis Source Code'). 

Arx Fatalis Source Code is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Arx Fatalis Source Code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Arx Fatalis Source Code.  If not, see 
<http://www.gnu.org/licenses/>.

In addition, the Arx Fatalis Source Code is also subject to certain additional terms. You should have received a copy of these 
additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Arx 
Fatalis Source Code. If not, please request a copy in writing from Arkane Studios at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing Arkane Studios, c/o 
ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/

#include "graphics/spells/Spells02.h"

#include <climits>

#include "core/Core.h"
#include "core/GameTime.h"

#include "game/EntityManager.h"
#include "game/Player.h"
#include "game/Spells.h"

#include "graphics/particle/Particle.h"
#include "graphics/particle/ParticleParams.h"
#include "graphics/particle/ParticleSystem.h"

#include "scene/Light.h"
#include "scene/Interactive.h"

CHeal::CHeal() {
	SetDuration(1000);
	ulCurrentTime = ulDuration + 1;

	pPS = new ParticleSystem();
}

CHeal::~CHeal() {
	delete pPS;
	pPS = NULL;
}

void CHeal::Create() {
	
	SetAngle(MAKEANGLE(player.angle.getPitch()));
	
	if(spells[spellinstance].m_caster == 0) {
		eSrc = player.pos;
	} else {
		eSrc = entities[spells[spellinstance].m_caster]->pos;
	}
	
	pPS->lLightId = GetFreeDynLight();

	if(lightHandleIsValid(pPS->lLightId)) {
		EERIE_LIGHT * light = lightHandleGet(pPS->lLightId);
		
		light->intensity = 2.3f;
		light->fallstart = 200.f;
		light->fallend   = 350.f;
		light->rgb.r = 0.4f;
		light->rgb.g = 0.4f;
		light->rgb.b = 1.0f;
		light->pos.x = eSrc.x;
		light->pos.y = eSrc.y - 50.f;
		light->pos.z = eSrc.z;
		light->duration = 200;
		light->extras = 0;
	}

	pPS->SetPos(eSrc);
	ParticleParams cp;
	memset(&cp, 0, sizeof(cp));
	cp.m_nbMax = 350;
	cp.m_life = 800;
	cp.m_lifeRandom = 2000;
	cp.m_pos = Vec3f(100, 200, 100);
	cp.m_direction = Vec3f(0, -10, 0) * 0.1f;
	cp.m_angle = radians(5);
	cp.m_speed = 120;
	cp.m_speedRandom = 84;
	cp.m_gravity = Vec3f(0, -10, 0);
	cp.m_flash = 0;
	cp.m_rotation = 1.0f / (101 - 80);

	cp.m_startSize = 8;
	cp.m_startSizeRandom = 8;
	cp.m_startColor = Color4f(205, 205, 255, 245);
	cp.m_startColorRandom = Color4f(50, 50, 0, 10);

	cp.m_endSize = 6;
	cp.m_endSizeRandom = 4;
	cp.m_endColor[0] = 20;
	cp.m_endColor[1] = 20;
	cp.m_endColor[2] = 30;
	cp.m_endColor[3] = 0;
	cp.m_endColorRandom[0] = 0;
	cp.m_endColorRandom[1] = 0;
	cp.m_endColorRandom[2] = 40;
	cp.m_endColorRandom[3] = 0;
	
	cp.m_blendMode = RenderMaterial::Additive;

	pPS->SetParams(cp);
	pPS->ulParticleSpawn = PARTICLE_CIRCULAR | PARTICLE_BORDER;
	pPS->SetTexture("graph/particles/heal_0005", 0, 100);
}

void CHeal::Update(unsigned long aulTime)
{
	ulCurrentTime += aulTime;

	if(ulCurrentTime >= ulDuration)
		return;
	
	if(spells[spellinstance].m_caster == 0) {
		eSrc = player.pos;
	} else if(ValidIONum(spells[spellinstance].m_target)) {
		eSrc = entities[spells[spellinstance].m_target]->pos;
	}
	
	if(!lightHandleIsValid(pPS->lLightId))
		pPS->lLightId = GetFreeDynLight();

	if(lightHandleIsValid(pPS->lLightId)) {
		EERIE_LIGHT * light = lightHandleGet(pPS->lLightId);
		
		light->intensity = 2.3f;
		light->fallstart = 200.f;
		light->fallend   = 350.f;
		light->rgb.r = 0.4f;
		light->rgb.g = 0.4f;
		light->rgb.b = 1.0f;
		light->pos.x = eSrc.x;
		light->pos.y = eSrc.y - 50.f;
		light->pos.z = eSrc.z;
		light->duration = 200;
		light->extras = 0;
	}

	unsigned long ulCalc = ulDuration - ulCurrentTime ;
	arx_assert(ulCalc <= LONG_MAX);
	long ff = static_cast<long>(ulCalc);

	if(ff < 1500) {
		pPS->ulParticleSpawn = PARTICLE_CIRCULAR;
		pPS->m_parameters.m_gravity = Vec3f_ZERO;

		std::list<Particle *>::iterator i;

		for(i = pPS->listParticle.begin(); i != pPS->listParticle.end(); ++i) {
			Particle * pP = *i;

			if(pP->isAlive()) {
				pP->fColorEnd[3] = 0;

				if(pP->ulTime + ff < pP->ulTTL) {
					pP->ulTime = pP->ulTTL - ff;
				}
			}
		}
	}

	pPS->SetPos(eSrc);
	pPS->Update(aulTime);
}

void CHeal::Render() {
	if(ulCurrentTime >= ulDuration)
		return;

	pPS->Render();
}

