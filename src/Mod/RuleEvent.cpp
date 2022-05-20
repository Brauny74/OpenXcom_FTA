/*
 * Copyright 2010-2019 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "RuleEvent.h"
#include "../Engine/Exception.h"

namespace OpenXcom
{

RuleEvent::RuleEvent(const std::string &name) : _name(name), _background("BACK13.SCR"), _city(false), _points(0), _funds(0), _loyalty(0),
												_spawnedPersons(0), _timer(30), _timerRandom(0), _backgroundColor(0)
{
}

/**
 * Loads the event definition from YAML.
 * @param node YAML node.
 */
void RuleEvent::load(const YAML::Node &node)
{
	if (const YAML::Node &parent = node["refNode"])
	{
		load(parent);
	}
	_name = node["name"].as<std::string>(_name);
	_description = node["description"].as<std::string>(_description);
	_background = node["background"].as<std::string>(_background);
	_music = node["music"].as<std::string>(_music);
	_regionList = node["regionList"].as<std::vector<std::string> >(_regionList);
	_city = node["city"].as<bool>(_city);
	_points = node["points"].as<int>(_points);
	_funds = node["funds"].as<int>(_funds);
	_loyalty = node["loyalty"].as<int>(_loyalty);
	_spawnedPersons = node["spawnedPersons"].as<int>(_spawnedPersons);
	_spawnedPersonType = node["spawnedPersonType"].as<std::string>(_spawnedPersonType);
	_spawnedPersonName = node["spawnedPersonName"].as<std::string>(_spawnedPersonName);
	_removedCovertOperationsList = node["removedCovertOperationsList"].as<std::vector<std::string> >(_removedCovertOperationsList);
	if (node["spawnedSoldier"])
	{
		_spawnedSoldier = node["spawnedSoldier"];
	}
	_reputationScore = node["reputationScore"].as<std::map<std::string, int>>(_reputationScore);
	_everyMultiItemList = node["everyMultiItemList"].as<std::map<std::string, int> >(_everyMultiItemList);
	_everyItemList = node["everyItemList"].as<std::vector<std::string> >(_everyItemList);
	_randomItemList = node["randomItemList"].as<std::vector<std::string> >(_randomItemList);
	if (node["weightedItemList"])
	{
		_weightedItemList.load(node["weightedItemList"]);
	}
	_researchList = node["researchList"].as<std::vector<std::string> >(_researchList);
	_interruptResearch = node["interruptResearch"].as<std::string>(_interruptResearch);
	_timer = node["timer"].as<int>(_timer);
	_timerRandom = node["timerRandom"].as<int>(_timerRandom);
	_backgroundColor = node["backgroundColor"].as<int>(_backgroundColor);
	if (const YAML::Node& customAnswers = node["customAnswers"])
	{
		if (customAnswers.size() > 4)
		{
			throw Exception("Geoscape Event named: '" + this->getName() + "' has more than 4 custom answers, this is not allowed!");
		}
		if (customAnswers.size() < 2)
		{
			throw Exception("Geoscape Event named: '" + this->getName() + "' has less than 2 custom answers, this is not allowed!");
		}
		for (YAML::const_iterator i = customAnswers.begin(); i != customAnswers.end(); ++i)
		{

			_answers[i->first.as<int>()].load(i->second);
		}
	}
}

}
