#include "Events.h"

// {905E2283-63FF-4b6e-A008-FE5E7CDA6612}

//0x905e2283, 0x63ff, 0x4b6e, 0xa0, 0x8, 0xfe, 0x5e, 0x7c, 0xda, 0x66, 0x12);


// Event GUIDs
// Engine-level Events
const EventType EventData_Engine_Loaded::sk_EventType(0x91a7e0ee);

// Actor Events
const EventType EventData_Destroy_Actor::sk_EventType(0x905e2283);
const EventType EventData_New_Actor::sk_EventType(0xc3926776);

const EventType EventData_New_Render_Component::sk_EventType(0xaf4aff75);
