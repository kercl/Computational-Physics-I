#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <vector>

class snapshot {
		static std::vector<snapshot*> logs;
	protected:
		virtual void do_snapshot() = 0;
	public:
		virtual void update() = 0;
		virtual void force_snapshot() = 0;
		
		static void register_logger(snapshot* sn);
		static void evoke();
};

class fixed_number_snapshot: public snapshot {
		int counter, limit;
	public:
		fixed_number_snapshot(int s, int n): 
			counter(0) {
			if(s < n)
				n = s;
			
			limit = s / (n - 1);
		}
		
		int get_counter();
		void force_snapshot();
		
		void update();
};

class regular_interval_snapshot: public snapshot {
		int counter, deltan;
	public:
		regular_interval_snapshot(int dn): 
			counter(0), deltan(dn) {}
		
		int get_counter();
		void force_snapshot();
		
		void update();
};

class fixed_point_snapshot: public snapshot {
		std::vector<int> points;
		int counter;
	public:
		fixed_point_snapshot(): counter(0) {}
	
		void add_snapshot_point(int a);
		int get_counter();
		void force_snapshot();
		
		void update();
};

#endif

