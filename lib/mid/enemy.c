// Copyright © 2011 Steve McCoy and Ethan Burns
// Licensed under the MIT License. See LICENSE for details.
#include "../../include/mid.h"
#include "enemy.h"
#include <stdio.h>

_Bool enemyldresrc(void){
	untihit = resrcacq(sfx, "sfx/hit.wav", 0);
	if(!untihit) return 0;
	untiimg = resrcacq(imgs, "img/unti.png", 0);
	if(!untiimg) return 0;
	thuimg = resrcacq(imgs, "img/thu.png", 0);
	if(!thuimg) return 0;
	splatimg = resrcacq(imgs, "img/splat.png", 0);
	if(!splatimg) return 0;
	nousimg = resrcacq(imgs, "img/nous.png", 0);
	if(!nousimg) return 0;
	grenduimg = resrcacq(imgs, "img/grendu.png", 0);
	if(!grenduimg) return 0;
	daimg = resrcacq(imgs, "img/da.png", 0);
	if(!daimg) return 0;

	return 1;
}

typedef struct Enemymt Enemymt;
struct Enemymt{
	_Bool (*init)(Enemy *, int, int);
	void (*free)(Enemy*);
	void (*update)(Enemy*, Player*, Zone*);
	void (*draw)(Enemy*, Gfx*);
	_Bool (*scan)(char *, Enemy *);
	_Bool (*print)(char *, size_t, Enemy *);
};

#define ENEMYMT(e) e##init, e##free, e##update, e##draw, e##scan, e##print

static Enemymt mt[] = {
	[EnemyUnti] = { ENEMYMT(unti) },
	[EnemyNous] = { ENEMYMT(nous) },
	[EnemyDa] = { ENEMYMT(da) },
	[EnemyThu] = { ENEMYMT(thu) },
	[EnemyGrendu] = { ENEMYMT(grendu) },
	[EnemySplat] = { ENEMYMT(splat) },
};

_Bool enemyinit(Enemy *e, EnemyID id, int x, int y){
	if(id == 0 || id >= EnemyMax)
		return 0;

	e->id = id;
	bodyinit(&e->body, x * Twidth, y * Theight, Twidth, Theight);

	return mt[id].init(e, x, y);
}

void enemyfree(Enemy *e){
	if(!e->id)
		return;
	mt[e->id].free(e);
	e->id = 0;
	e->hp = 0;
}

void enemyupdate(Enemy *e, Player *p, Zone *z){
	if(e->id) mt[e->id].update(e, p, z);
}

void enemydraw(Enemy *e, Gfx *g){
	if(e->id) mt[e->id].draw(e, g);
}

_Bool enemyscan(char *buf, Enemy *e){
	int id;
	// need to take a peek at the ID to dispatch the correct scan method.
	if (sscanf(buf, " %d", &id) != 1)
		return 0;
	if (!mt[id].scan)
		return defaultscan(buf, e); 
	return mt[id].scan(buf, e);
}

_Bool enemyprint(char *buf, size_t s, Enemy *e){
	if (!mt[e->id].print)
		return defaultprint(buf, s, e); 
	return mt[e->id].print(buf, s, e);
}

_Bool defaultscan(char *buf, Enemy *e){
	return scangeom(buf, "dyd", &e->id, &e->body, &e->hp);
}

_Bool defaultprint(char *buf, size_t sz, Enemy *e){
	return printgeom(buf, sz, "dyd", e->id, e->body, e->hp);
}
