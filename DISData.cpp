#include "DISData.h"

// consturct DIS::EntityID as output
DIS::EntityID getEntityID(ush site, ush application, ush ID)
{
	DIS::EntityID entity_id;
	entity_id.setSite(site);
	entity_id.setApplication(application);
	entity_id.setEntity(ID);
	return entity_id;
}

// based on DATA_TYPE return presumed entity type
// check SISO document(2022) for type https://www.sisostds.org/DesktopModules/Bring2mind/DMX/API/Entries/Download?Command=Core_Download&EntryId=46172&PortalId=0&TabId=105
DIS::EntityType getEntityType(DATA_TYPE type)
{
	DIS::EntityType entity_type;    
	
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
		entity_type.setDomain(2); // set default air
		entity_type.setCountry(0);
		entity_type.setCategory(0);
		break;
	}
	return entity_type;
}

 // return DIS::DeadReckoningParameter based on parameter
DIS::DeadReckoningParameter getDRP(Example::DeadReckoningModel p) 
{
	DIS::DeadReckoningParameter drp;
	drp.setDeadReckoningAlgorithm(p);
	return drp;
}

// constructor warpper
Example::DISData::DISData(ush site, ush application, ush ID, DATA_TYPE t, Example::DeadReckoningModel p)
	: DISData(0, 0, t,
		getEntityID(site, application, ID), 
		getEntityType(t),
		getDRP(p))
{
	data_type = t;
}


Example::DISData::DISData(uch version, 
	uch exerciseid, 
	DATA_TYPE t,
	const DIS::EntityID& entity_id, 
	const DIS::EntityType& type,
	const DIS::DeadReckoningParameter& drp)
{
	data_type = t;
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
	

	set_pos(1.0f, 0.0f, 0.0f);
	set_ori(0.0f, 0.0f, 0.0f);
	set_vel(0.0f, 0.0f, 0.0f);
	frame_stamp = 3;

	update_pdu();
	//currenlt not considering ArticulationParameters
	// TODO: ArticulationParameters
	espdu->setLength(espdu->getMarshalledSize());
	return;
}


//default deconstructor
Example::DISData::~DISData()
{
	espdu.reset();
	return;
}

// set the position
// if need update to espdu call update_pdu()
void Example::DISData::set_pos(float x, float y, float z)
{
	position.setX(x);
	position.setY(y);
	position.setZ(z);
	return;
}

// set the orientation
// if need update to espdu call update_pdu()
void Example::DISData::set_ori(float psi, float theta, float phi)
{
	orientation.setPsi(psi);
	orientation.setTheta(theta);
	orientation.setPhi(phi);
	return;
}

// set the velocity 
// if need update to espdu call update_pdu()
void Example::DISData::set_vel(float x, float y, float z)
{
	velocity.setX(x);
	velocity.setY(y);
	velocity.setZ(z);
	return;
}

// set the frame stamp
// if need update to espdu call update_pdu()
void Example::DISData::set_time(unsigned int t)
{
	frame_stamp = t;
	return;
}

// set espdu from pos/ori/vel/time_dysmp saved
void Example::DISData::update_pdu()
{
	espdu->setEntityLocation(position);
	espdu->setEntityOrientation(orientation);
	espdu->setEntityLinearVelocity(velocity);
	espdu->setTimestamp(frame_stamp);
	return;
}

//  write espdu into buffer to be sended
void Example::DISData::marshal(DIS::DataStream& buffer)
{
	espdu->marshal(buffer);
	return;
}

// get self espdu entity id
uch Example::DISData::get_entity_id()
{
	return espdu->getEntityID().getEntity();
}


// currently our raw data for missle/interceptor
// we only use first 2 strings of vector for position and orientation
// for non-aboved DATA_TYPE we only paser position
void Example::DISData::parse_raw_data(const std::vector<std::string>& raw_data)
{
	std::vector<float> pos;
	
	pos = strV2floatV(splitString(raw_data[0], ','));
	
	set_pos(pos[0], pos[1], pos[2]);
	//print_pos();
	// none missle/interceptor type
	if (data_type != MISSILE && data_type != INTERCEPTOR) {
		update_pdu();
		return;
	}

	pos.clear();
	// rotation information
	pos = strV2floatV(splitString(raw_data[1], ','));
	set_ori(pos[0], pos[1], pos[2]);

	update_pdu();
	return;

}

// print position XYZ
void Example::DISData::print_pos()
{
	LOG_WARNING(
		std::to_string(position.getX()) + " "
		+ std::to_string(position.getY()) + " "
		+ std::to_string(position.getZ()));
	
	return;
}

// print orientation Psi Theta Phi
void Example::DISData::print_ori()
{
	LOG_WARNING(
		std::to_string(orientation.getPsi()) + " "
		+ std::to_string(orientation.getTheta()) + " "
		+ std::to_string(orientation.getPhi()));

	return;
}

// print linear velocity XYZ
void Example::DISData::print_vel()
{
	LOG_WARNING(
		std::to_string(velocity.getX()) + " "
		+ std::to_string(velocity.getY()) + " "
		+ std::to_string(velocity.getZ()));

	return;
}