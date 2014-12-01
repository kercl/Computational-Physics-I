#include "snapshot.h"

void fixed_number_snapshot::update() {
	if(counter % limit == 0)
		do_snapshot();
	
	counter++;
}

int fixed_number_snapshot::get_counter() {
	return counter;
}

void fixed_number_snapshot::force_snapshot() {
	do_snapshot();
	counter++;
}

void regular_interval_snapshot::update() {
	if(counter % deltan == 0)
		do_snapshot();
	
	counter++;
}

int regular_interval_snapshot::get_counter() {
	return counter;
}

void regular_interval_snapshot::force_snapshot() {
	do_snapshot();
	counter++;
}

void fixed_point_snapshot::add_snapshot_point(int a) {
	points.push_back(a);
}

void fixed_point_snapshot::update() {
	for(int i = 0; i < points.size(); ++i)
		if(points[i] == counter)
			do_snapshot();
	
	counter++;
}

int fixed_point_snapshot::get_counter() {
	return counter;
}

void fixed_point_snapshot::force_snapshot() {
	do_snapshot();
	counter++;
}

void snapshot::register_logger(snapshot* sn) {
	logs.push_back(sn);
}

void snapshot::evoke() {
	for(int i = 0; i < logs.size(); ++i)
		logs[i]->update();
}

std::vector<snapshot*> snapshot::logs;

