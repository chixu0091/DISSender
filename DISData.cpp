#include "DISData.h"


DIS::EntityID getEntityID(ush site, ush application, ush ID)
{
	DIS::EntityID entity_id;
	entity_id.setSite(site);
	entity_id.setApplication(application);
	entity_id.setEntity(ID);
	return entity_id;
}


DIS::EntityType getEntityType(DATA_TYPE type)
{
	DIS::EntityType entity_type;    
	// check SISO document(2022) for type https://www.sisostds.org/DesktopModules/Bring2mind/DMX/API/Entries/Download?Command=Core_Download&EntryId=46172&PortalId=0&TabId=105
	switch (type) {
	case MISSILE:
	case INTERCEPTOR:
		
		//assume missile is hq2j: 2.1.45.1.1.2
		entity_type.setEntityKind(2);   //Munition
		entity_type.setDomain(1);       //Anti-Air
		entity_type.setCountry(45);     //PRC
		entity_type.setCategory(1);     //guided missile
		entity_type.setSubcategory(1);
		entity_type.setSpecific(2);
		entity_type.setExtra(0);
		break;
	case RADAR_M:
	case RADAR_T:
		//assume radar is VERA: 9.1.267.3.16.3 
		entity_type.setEntityKind(9);   
		entity_type.setDomain(1);       
		entity_type.setCountry(267);    
		entity_type.setCategory(3);     
		entity_type.setSubcategory(16);
		entity_type.setSpecific(3);
		entity_type.setExtra(0);
		break;
	case TARGET:
	case OTHER:
	default:
		//TODO: Specify Target type by its location, like air
		// land device or anything else
		entity_type.setEntityKind(0); //Other
		entity_type.setDomain(2);
		entity_type.setCountry(0);
		entity_type.setCategory(0);
		break;
	}
	return entity_type;
}

DIS::DeadReckoningParameter getDRP(Example::DeadReckoningModel p) 
{
	DIS::DeadReckoningParameter drp;
	drp.setDeadReckoningAlgorithm(p);
	return drp;
}

//default constructor
Example::DISData::DISData(ush site, ush application, ush ID, DATA_TYPE t, Example::DeadReckoningModel p)
	: DISData(0, 0, 
		getEntityID(site, application, ID), 
		getEntityType(t),
		getDRP(p))
{
}

Example::DISData::DISData(uch version, uch exerciseid,
	const DIS::EntityID& entity_id, 
	const DIS::EntityType& type,
	const DIS::DeadReckoningParameter& drp)
{
	espdu = std::make_shared<DIS::EntityStatePdu>();
	

	//set default DIS version to 6
	if (version != 5 && version != 6 && version != 7) {
		espdu->setProtocolVersion(6);
	}
	else {
		espdu->setProtocolVersion(version);
	}
	espdu->setExerciseID(exerciseid);


	espdu->setEntityID(entity_id);
	espdu->setEntityType(type);
	espdu->setDeadReckoningParameters(drp);
	

	update_ori(0.0f, 0.0f, 0.0f);
	update_ori(0.0f, 0.0f, 0.0f);
	update_vel(0.0f, 0.0f, 0.0f);
	frame_stamp = 3;

	update_pdu();
	return;
}



//default deconstructor
Example::DISData::~DISData()
{
	espdu.reset();
	return;
}

// set the position
void Example::DISData::update_pos(float x, float y, float z)
{
	position.setX(x);
	position.setY(y);
	position.setZ(z);
	return;
}

// set the orientation
void Example::DISData::update_ori(float psi, float theta, float phi)
{
	orientation.setPsi(psi);
	orientation.setTheta(theta);
	orientation.setPhi(phi);
	return;
}

// set the velocity 
void Example::DISData::update_vel(float x, float y, float z)
{
	velocity.setX(x);
	velocity.setY(y);
	velocity.setZ(z);
	return;
}

// set the frame stamp
void Example::DISData::update_time(unsigned int t)
{
	frame_stamp = t;
	return;
}

void Example::DISData::update_pdu()
{
	espdu->setEntityLocation(position);
	espdu->setEntityOrientation(orientation);
	espdu->setEntityLinearVelocity(velocity);
	espdu->setTimestamp(frame_stamp);
	return;
}

void Example::DISData::marshal(DIS::DataStream& buffer)
{
	espdu->marshal(buffer);
	return;
}

uch Example::DISData::get_entity_id()
{
	return espdu->getEntityID().getEntity();
}