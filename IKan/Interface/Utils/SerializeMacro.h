//
//  SerializeMacro.h
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#pragma once

#define IK_SERIALIZE_PROPERTY(propName, propVal, outputNode) outputNode << YAML::Key << #propName << YAML::Value << propVal
#define IK_DESERIALIZE_PROPERTY(propName, destination, node, defaultValue) destination = node[#propName] ? node[#propName].as<decltype(defaultValue)>() : defaultValue
