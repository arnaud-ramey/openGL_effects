/*
 * drawing functions
 */

void setMaterial(RTGMaterial *material) {
	/* Sätt material här */
	if (material->texFile != NULL) {
		load_texture(material->texFile, TEX_UNIT_1);
	}
	//glMaterialfv(material->)
	//	glColor3fv(material->amb);
	//glColor3f(1,0,0 );
}

void drawObject(RTGFile *file, RTGObject *obj, bool useTextures) {
	unsigned int i, j;
	RTGVertex *v;
	RTGVec3 *n;
	RTGVec3 *p;
	RTGVec2 *t;

	if (useTextures)
		setMaterial(file->mat[obj->mat]);

	for (i = 0; i < obj->npoly; i++) {
		glBegin( GL_POLYGON);
		for (j = 0; j < obj->poly[i]->nvert; j++) {
			v = &obj->poly[i]->v[j];
			n = &obj->norm[v->n];
			t = &obj->tex[v->t];
			p = &obj->pos[v->p];
			glNormal3f(n->x, n->y, n->z);
			glTexCoord2f(t->x, t->y);
			glTexCoord2f(t->x, t->y);
			glVertex3f(p->x, p->y, p->z);
		}
		glEnd();
	}
}

void drawNode(RTGFile *file, RTGNode *node, bool useTextures) {
	unsigned int i;

	/* Om noden har geometri, rita den */
	if (node->obj) {
		drawObject(file, node->obj, useTextures);
	}
	/* Om noden har barn, rita dem */
	for (i = 0; i < node->nchildren; i++) {
		drawNode(file, node->children[i], useTextures);
	}

}

void drawRTG(RTGFile *file, bool useTextures, double x, double y, double z,
		double scale, double angle) {
	unsigned int i;

	glPushMatrix();
	/* Applicera transformation här! */
	glTranslated(x, y, z);
	glScaled(scale, scale, scale);
	glRotated(angle, 0, 1, 0);

	/* Rita alla toppniv<8C>noder */
	for (i = 0; i < file->nnodes; i++) {
		drawNode(file, file->nodes[i], useTextures);
	}

	glPopMatrix();

}

void drawRTG(RTGFile *file, bool useTextures) {
	drawRTG(file, useTextures, 0, 0, 0, 1, 0);
}

class Hero {
public:
	RTGFile *rtg_file;
	double scale;
	double x, y, z, angle;

	Hero(const char* filename) {
		rtg_file = RTG_Parse(filename, 1);
	}

	void setPos(double X, double Y, double Z, double Angle) {
		x = X;
		y = Y;
		z = Z;
		angle = Angle;
	}
};

/*!
 * draw the hero
 */
static void drawHero(Hero* h, bool useTextures) {
	drawRTG(h->rtg_file, useTextures, h->x, h->y, h->z, h->scale, h->angle);
}

