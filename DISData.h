#pragma once

#include <dis6/Vector3Double.h>
#include <dis6/Vector3Float.h>
#include <dis6/Orientation.h>

#include <dis6/EntityStatePdu.h>

#include "COMMON.h"
#include "Logging.h"
namespace Example
{
	class DISData
	{
	public:
		DISData(ush site, ush application, ush ID, DATA_TYPE t, Example::DeadReckoningModel p);
		DISData(uch version, uch exerciseid, DATA_TYPE t,
			const DIS::EntityID& entity_id, 
			const DIS::EntityType& type,
			const DIS::DeadReckoningParameter& drp);
		~DISData();

		void set_pos(float x, float y, float z);
		void set_ori(float psi, float theta, float phi);
		void set_vel(float x, float y, float z);
		void set_time(unsigned int t);

		void update_pdu();
		void marshal(DIS::DataStream& buffer);

		uch get_entity_id();

	private:
		std::shared_ptr<DIS::EntityStatePdu> espdu;
		DIS::Vector3Double position;
		DIS::Orientation orientation;
		DIS::Vector3Float velocity; //linear velocity

		unsigned int frame_stamp;
		DATA_TYPE data_type;

	public:
		void parse_raw_data(const std::vector<std::string>& raw_data);

		void print_pos();
		void print_ori();
		void print_vel();

		

		
	};
}