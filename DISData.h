#pragma once

#include <memory>

#include <dis6/Vector3Double.h>
#include <dis6/Vector3Float.h>
#include <dis6/Orientation.h>

#include <dis6/EntityStatePdu.h>


typedef unsigned char uch;
typedef unsigned short ush;

enum DATA_TYPE {
	OTHER = -1,
	MISSILE,        //攻击导弹
	INTERCEPTOR,    //拦截导弹
	RADAR_M,        //导弹雷达
	RADAR_T,        //目标雷达
	TARGET          //目标
};


namespace Example
{
	enum DeadReckoningModel
	{
		STATIC = 1,
		DRM_FPW,
		DRM_RPW,
		DRM_RVW,
		DRM_FVW,
		DRM_FPB,
		DRM_RPB,
		DRM_RVB,
		DRM_FVB,
	};


	class DISData
	{
	public:
		DISData(ush site, ush application, ush ID, DATA_TYPE t, Example::DeadReckoningModel p);
		DISData(uch version, uch exerciseid,
			const DIS::EntityID& entity_id, 
			const DIS::EntityType& type,
			const DIS::DeadReckoningParameter& drp);
		~DISData();

		void update_pos(float x, float y, float z);
		void update_ori(float psi, float theta, float phi);
		void update_vel(float x, float y, float z);
		void update_time(unsigned int t);

		void update_pdu();
		void marshal(DIS::DataStream& buffer);

		uch get_entity_id();
	private:
		std::shared_ptr<DIS::EntityStatePdu> espdu;
		DIS::Vector3Double position;
		DIS::Orientation orientation;
		DIS::Vector3Float velocity; //linear velocity

		unsigned int frame_stamp;
	};
}