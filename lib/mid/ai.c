// Copyright © 2011 Steve McCoy and Ethan Burns
// Licensed under the MIT License. See LICENSE for details.
#include "../../include/mid.h"

static void dojump(Enemy*,Player*,Lvl*);
static void walk(Enemy*,Player*,Lvl*);
static void patrol(Enemy*,Player*,Lvl*);
static void chase(Enemy*,Player*,Lvl*);
static void hunt(Enemy*,Player*,Lvl*);

void aijumper(Ai *ai, double jv){
	ai->update = dojump;
	ai->mv = (Point){0, jv};
	ai->awdst = 0;
}

void aiwalker(Ai *ai, double wv){
	ai->update = walk;
	ai->mv = (Point){wv, 0};
	ai->awdst = 0;
}

void aipatroller(Ai *ai, double wv){
	ai->update = patrol;
	ai->mv = (Point){wv, 0};
	ai->awdst = 0;
}

void aichaser(Ai *ai, double wv, double awdst){
	ai->update = chase;
	ai->mv = (Point){wv, 0};
	ai->awdst = awdst;
}

void aihunter(Ai *ai, double jv, double wv, double awdst){
	ai->update = hunt;
	ai->mv = (Point){wv, jv};
	ai->awdst = awdst;
}

static void dojump(Enemy *e, Player *p, Lvl *lvl){
	if(!e->b.fall){
		e->b.vel.y = -e->ai.mv.y;
		e->b.fall = 1;
	}
}

static void walk(Enemy *e, Player *p, Lvl *lvl){
	double wx = e->ai.mv.x;

	if(e->b.bbox.a.x == e->ai.lastp.x)
		e->ai.mv.x = -wx;

	e->ai.lastp = e->b.bbox.a;

	e->b.vel.x = e->ai.mv.x;
}

static void patrol(Enemy *e, Player *p, Lvl *lvl){
	double wx = e->ai.mv.x;
	double bw = e->b.bbox.b.x - e->b.bbox.a.x;
	double s = wx < 0 ? -1 : 1;

	Rect nextlow = {
		vecadd(e->b.bbox.a, (Point){s*bw,32}),
		vecadd(e->b.bbox.b, (Point){s*bw,32})
	};
	if(!(lvlmajorblk(lvl, nextlow).flags & Tilecollide) || e->b.bbox.a.x == e->ai.lastp.x)
		e->ai.mv.x = -wx;

	e->ai.lastp = e->b.bbox.a;

	e->b.vel.x = e->ai.mv.x;
}

static void chase(Enemy *e, Player *p, Lvl *lvl){
	if(dist(e->b.bbox.a, p->body.bbox.a) > e->ai.awdst)
		return; //unaware

	double wx = e->ai.mv.x;

	if(p->body.bbox.a.x < e->b.bbox.a.x)
		wx = -wx;

	e->ai.lastp = e->b.bbox.a;

	e->b.vel.x = wx;
}

static void hunt(Enemy *e, Player *p, Lvl *lvl){

}
