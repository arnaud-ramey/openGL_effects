/*
 * mirror.cpp
 *
 *  Created on: Sep 20, 2009
 *      Author: arnaud
 */
/*!
 * 1st pass of the mirror
 *
 * We draw the reflection area
 */
void mirror_first_pass() {

	// Disable writting in color buffer
	glColorMask(false, false, false, false);
	// Disable writting in depth buffer
	glDisable( GL_DEPTH_TEST);

	//Enable Stencil test
	glEnable( GL_STENCIL_TEST);
	//Test always success, value written 1
	glStencilFunc(GL_ALWAYS, 1, 1);
	//Stencil & Depth test passes => replace existing value
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glClearStencil(0);

	// Draw ground
	drawFloor(true);

	// Enable writting in depth buffer
	glEnable(GL_DEPTH_TEST);
	// Enable writting in color buffer
	glColorMask(1, 1, 1, 1);
}

/*!
 * Here we draw the reflection in the mask area, defined in the stencil buffer.
 */
void mirror_draw_reflection(Hero* h) {
	// The stencil function tests if value in the stencil buffer is 1,
	// to draw only where stencil buffer contains a '1'.
	// When the test succeed, we do nothing (keep current value).
	glEnable( GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Stencil buffer read only

	glPushMatrix();
	glScalef(1.0f, -1.0f, 1.0f); //Mirror Y
	//	glEnable(GL_LIGHTING);

	unload_texture_unit(TEX_UNIT_1);
	drawHero(h, true);

	//	glDisable(GL_LIGHTING);
	glPopMatrix();

	glDisable(GL_STENCIL_TEST); //Disable Stencil test
}

/*!
 * Now, we draw the reflection area in color buffer.
 * We uses blending to mix the reflection with the ground (keep only 20% of the reflection).
 */
void mirror_second_pass() {
	//We uses blending to mix the reflection with the ground (keep only 20% of the reflection).
	glEnable( GL_BLEND);
	// source = floor
	// dest = hero
	//glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glBlendFunc(GL_ONE, GL_ONE);
	drawFloor(true);
	glDisable(GL_BLEND);
	//	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

/*!
 * activate the mirror
 */
void mirror(vector<Hero*>* heroesToReflect) {
	mirror_first_pass();

	for (vector<Hero*>::iterator h = heroesToReflect->begin(); h
			< heroesToReflect->end(); ++h) {
		mirror_draw_reflection(*h);
	}

	mirror_second_pass();

	for (vector<Hero*>::iterator h = heroesToReflect->begin(); h
			< heroesToReflect->end(); ++h) {
		unload_texture_unit(TEX_UNIT_1);
		glEnable( GL_LIGHTING); //enable lighting
		glEnable( GL_LIGHT0); //enable lighting
		drawHero(*h, true);
	}
}

