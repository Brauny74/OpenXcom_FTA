/*
 * Copyright 2010-2017 OpenXcom Developers.
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
#include "SoldierInfoState.h"
#include "SoldierDiaryOverviewState.h"
#include <algorithm>
#include <sstream>
#include "../Engine/Game.h"
#include "../Engine/Action.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/Options.h"
#include "../Interface/Bar.h"
#include "../Interface/TextButton.h"
#include "../Interface/Text.h"
#include "../Interface/TextEdit.h"
#include "../Interface/ComboBox.h"
#include "../Engine/Surface.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/Base.h"
#include "../Savegame/Craft.h"
#include "../Savegame/CovertOperation.h"
#include "../Savegame/Soldier.h"
#include "../Engine/SurfaceSet.h"
#include "../Mod/Armor.h"
#include "../Mod/RuleSoldier.h"
#include "../Menu/ErrorMessageState.h"
#include "SellState.h"
#include "SoldierArmorState.h"
#include "SoldierBonusState.h"
#include "SackSoldierState.h"
#include "../Mod/RuleInterface.h"
#include "../Mod/RuleSoldier.h"
#include "../Mod/RuleSoldierBonus.h"
#include "../Savegame/SoldierDeath.h"

namespace OpenXcom
{
/**
 * Initializes all the elements in the Soldier Info screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from. NULL to use the dead soldiers list.
 * @param soldierId ID of the selected soldier.
 */
SoldierInfoState::SoldierInfoState(Base *base, size_t soldierId) : _base(base), _soldierId(soldierId), _soldier(0)
{
	if (_base == 0)
	{
		_list = _game->getSavedGame()->getDeadSoldiers();
		if (_soldierId >= _list->size())
		{
			_soldierId = 0;
		}
		else
		{
			_soldierId = _list->size() - (1 + _soldierId);
		}
	}
	else
	{
		_list = _base->getSoldiers();
	}

	_ftaUI = _game->getMod()->getIsFTAGame();
	_localChange = false;

	// Create objects
	_bg = new Surface(320, 200, 0, 0);
	_rank = new Surface(26, 23, 4, 4);
	_flag = new InteractiveSurface(40, 20, 275, 6);
	_btnPrev = new TextButton(28, 14, 0, 33);
	_btnOk = new TextButton(48, 14, 30, 33);
	_btnNext = new TextButton(28, 14, 80, 33);
	_btnArmor = new TextButton(110, 14, 130, 33);
	_btnBonuses = new TextButton(16, 14, 242, 33);
	_edtSoldier = new TextEdit(this, 210, 16, 40, 9);
	_btnSack = new TextButton(60, 14, 260, 33);
	if (_ftaUI)
	{
		_btnDiary = new TextButton(60, 14, 260, 33);
	}
	else
	{
		_btnDiary = new TextButton(60, 14, 260, 48);
	}
	_cbxRoles = new ComboBox(this, 60, 14, 260, 48, false);
	_txtRank = new Text(130, 9, 0, 48);
	_txtMissions = new Text(100, 9, 130, 48);
	_txtKills = new Text(100, 9, 200, 48);
	_txtStuns = new Text(60, 9, 260, 48);
	_txtCraft = new Text(130, 9, 0, 56);
	_txtRecovery = new Text(180, 9, 130, 56);
	_txtOperation = new Text(180, 9, 0, 64);
	_txtPsionic = new Text(150, 9, 0, 66);
	_txtDead = new Text(150, 9, 130, 33);

	defineStatLines();

	// Set palette
	setInterface("soldierInfo");

	add(_bg);
	add(_rank);
	add(_flag);
	add(_btnOk, "button", "soldierInfo");
	add(_btnPrev, "button", "soldierInfo");
	add(_btnNext, "button", "soldierInfo");
	add(_btnArmor, "button", "soldierInfo");
	add(_btnBonuses, "button", "soldierInfo");
	add(_edtSoldier, "text1", "soldierInfo");
	add(_btnSack, "button", "soldierInfo");
	add(_btnDiary, "button", "soldierInfo");
	add(_cbxRoles, "button", "soldierInfo");
	add(_txtRank, "text1", "soldierInfo");
	add(_txtMissions, "text1", "soldierInfo");
	add(_txtKills, "text1", "soldierInfo");
	add(_txtStuns, "text1", "soldierInfo");
	add(_txtCraft, "text1", "soldierInfo");
	add(_txtOperation, "text1", "soldierInfo");
	add(_txtRecovery, "text1", "soldierInfo");
	add(_txtPsionic, "text2", "soldierInfo");
	add(_txtDead, "text2", "soldierInfo");

	addStatLines();

	centerAllSurfaces();

	// Set up objects
	_game->getMod()->getSurface("BACK06.SCR")->blitNShade(_bg, 0, 0);

	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&SoldierInfoState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&SoldierInfoState::btnOkClick, Options::keyCancel);

	_btnPrev->setText("<<");
	if (_base == 0)
	{
		_btnPrev->onMouseClick((ActionHandler)&SoldierInfoState::btnNextClick);
		_btnPrev->onKeyboardPress((ActionHandler)&SoldierInfoState::btnNextClick, Options::keyBattlePrevUnit);
	}
	else
	{
		_btnPrev->onMouseClick((ActionHandler)&SoldierInfoState::btnPrevClick);
		_btnPrev->onKeyboardPress((ActionHandler)&SoldierInfoState::btnPrevClick, Options::keyBattlePrevUnit);
	}

	_btnNext->setText(">>");
	if (_base == 0)
	{
		_btnNext->onMouseClick((ActionHandler)&SoldierInfoState::btnPrevClick);
		_btnNext->onKeyboardPress((ActionHandler)&SoldierInfoState::btnPrevClick, Options::keyBattleNextUnit);
	}
	else
	{
		_btnNext->onMouseClick((ActionHandler)&SoldierInfoState::btnNextClick);
		_btnNext->onKeyboardPress((ActionHandler)&SoldierInfoState::btnNextClick, Options::keyBattleNextUnit);
	}

	_btnArmor->setText(tr("STR_ARMOR"));
	_btnArmor->onMouseClick((ActionHandler)&SoldierInfoState::btnArmorClick);

	_btnBonuses->setText(tr("STR_BONUSES_BUTTON")); // tiny button, default translation is " "
	_btnBonuses->onMouseClick((ActionHandler)&SoldierInfoState::btnBonusesClick);

	_edtSoldier->setBig();
	_edtSoldier->onChange((ActionHandler)&SoldierInfoState::edtSoldierChange);
	_edtSoldier->onMousePress((ActionHandler)&SoldierInfoState::edtSoldierPress);

	// Can't change nationality of dead soldiers
	if (_base != 0)
	{
		// Ignore also if flags are used to indicate number of kills
		if (_game->getMod()->getFlagByKills().empty())
		{
			_flag->onMouseClick((ActionHandler)&SoldierInfoState::btnFlagClick, SDL_BUTTON_LEFT);
			_flag->onMouseClick((ActionHandler)&SoldierInfoState::btnFlagClick, SDL_BUTTON_RIGHT);
		}
	}

	_btnSack->setText(tr("STR_SACK"));
	_btnSack->onMouseClick((ActionHandler)&SoldierInfoState::btnSackClick);
	if (_ftaUI)
	{
		_btnSack->setVisible(false);
		_btnSack->setX(0); //go away!
		_btnSack->setY(0);
		_btnSack->setWidth(0);
		_btnSack->setHeight(0);
	}

	_btnDiary->setText(tr("STR_DIARY"));
	_btnDiary->onMouseClick((ActionHandler)&SoldierInfoState::btnDiaryClick);
	_btnDiary->setVisible(Options::soldierDiaries);

	_rolesList.push_back("STR_SOLDIER");
	_rolesList.push_back("STR_PILOT");
	_rolesList.push_back("STR_AGENT");
	_rolesList.push_back("STR_SCIENTIST");
	_rolesList.push_back("STR_ENGINEER");
	_cbxRoles->setOptions(_rolesList, true);
	_cbxRoles->setSelected(0);
	_cbxRoles->onChange((ActionHandler)&SoldierInfoState::cbxRolesChange);
	_cbxRoles->setVisible(_ftaUI);

	_txtPsionic->setText(tr("STR_IN_PSIONIC_TRAINING"));

	nameBars();
}

/**
 *
 */
SoldierInfoState::~SoldierInfoState()
{

}

/**
 * Updates soldier stats when
 * the soldier changes.
 */
void SoldierInfoState::init()
{
	State::init();
	if (_list->empty())
	{
		_game->popState();
		return;
	}
	if (_soldierId >= _list->size())
	{
		_soldierId = 0;
	}
	_soldier = _list->at(_soldierId);
	_edtSoldier->setBig();
	_edtSoldier->setText(_soldier->getName());
	UnitStats *initial = _soldier->getInitStats();
	UnitStats *current = _soldier->getCurrentStats();

	bool hasBonus = _soldier->prepareStatsWithBonuses(_game->getMod()); // refresh all bonuses
	UnitStats withArmor = *_soldier->getStatsWithAllBonuses();
	_btnBonuses->setVisible(hasBonus);

	SoldierRole role = _soldier->getBestRole();
	// here we set default display, do not do this if rendering after combobox change!
	if (!_localChange) 
	{
		switch (role)
		{
		case OpenXcom::ROLE_SOLDIER:
			_cbxRoles->setSelected(0);
			break;
		case OpenXcom::ROLE_PILOT:
			_cbxRoles->setSelected(1);
			break;
		case OpenXcom::ROLE_AGENT:
			_cbxRoles->setSelected(2);
			break;
		case OpenXcom::ROLE_SCIENTIST:
			_cbxRoles->setSelected(3);
			break;
		case OpenXcom::ROLE_ENGINEER:
			_cbxRoles->setSelected(4);
			break;
		default:
			_cbxRoles->setSelected(0);
			break;
		}
	}

	SurfaceSet *texture = _game->getMod()->getSurfaceSet("BASEBITS.PCK");
	auto frame = texture->getFrame(_soldier->getRankSprite());
	if (_ftaUI)
	{
		frame = texture->getFrame(_soldier->getRoleRankSprite(role));
	}
	if (frame)
	{
		frame->blitNShade(_rank, 0, 0);
	}

	std::ostringstream flagId;
	flagId << "Flag";
	const std::vector<int> mapping = _game->getMod()->getFlagByKills();
	if (mapping.empty())
	{
		flagId << _soldier->getNationality() + _soldier->getRules()->getFlagOffset();
	}
	else
	{
		int index = 0;
		for (auto item : mapping)
		{
			if (_soldier->getKills() <= item)
			{
				break;
			}
			index++;
		}
		flagId << index + _soldier->getRules()->getFlagOffset();
	}
	Surface *flagTexture = _game->getMod()->getSurface(flagId.str().c_str(), false);
	_flag->clear();
	if (flagTexture != 0)
	{
		flagTexture->blitNShade(_flag, _flag->getWidth() - flagTexture->getWidth(), 0); // align right
	}

	_soldierInfoView->fillNumbers(
		*_soldier->getStatsWithAllBonuses(),
		_soldier->getCurrentStats(),
		_soldier->getInitStats()
	);

	updateVisibility();

	std::string wsArmor;
	if (_soldier->getArmor() == _soldier->getRules()->getDefaultArmor())
	{
		wsArmor= tr("STR_ARMOR_").arg(tr(_soldier->getArmor()->getType()));
	}
	else
	{
		wsArmor = tr(_soldier->getArmor()->getType());
	}

	_btnArmor->setText(wsArmor);

	_btnSack->setVisible(_game->getSavedGame()->getMonthsPassed() > -1 && !(_soldier->getCraft() && _soldier->getCraft()->getStatus() == "STR_OUT"));
	if (_soldier->getCovertOperation() != 0)
	{
		_btnSack->setVisible(false);
	}
	_txtRank->setText(tr("STR_RANK_").arg(tr(_soldier->getRankString(_ftaUI))));

	_txtMissions->setText(tr("STR_MISSIONS").arg(_soldier->getMissions()));

	_txtKills->setText(tr("STR_KILLS").arg(_soldier->getKills()));

	_txtStuns->setText(tr("STR_STUNS").arg(_soldier->getStuns()));
	_txtStuns->setVisible(!Options::soldierDiaries);

	std::string craft;
	if (_soldier->getCraft() == 0)
	{
		craft = tr("STR_NONE_UC");
	}
	else
	{
		craft = _soldier->getCraft()->getName(_game->getLanguage());
	}
	_txtCraft->setText(tr("STR_CRAFT_").arg(craft));

	if (_soldier->getCovertOperation() != 0)
	{
		_txtOperation->setVisible(true);
		_txtOperation->setText(tr("STR_OPERATION_").arg(tr(_soldier->getCovertOperation()->getOperationName())));
	}
	else
	{
		_txtOperation->setVisible(false);
	}

	auto recovery = _base ? _base->getSumRecoveryPerDay() : BaseSumDailyRecovery();
	auto getDaysOrInfinity = [&](int days)
	{
		if (days < 0)
		{
			return std::string{ "∞" };
		}
		else
		{
			return std::string{tr("STR_DAY", days)};
		}
	};
	if (_soldier->isWounded())
	{
		int recoveryTime = _soldier->getNeededRecoveryTime(recovery);
		_txtRecovery->setText(tr("STR_WOUND_RECOVERY").arg(getDaysOrInfinity(recoveryTime)));
	}
	else
	{
		_txtRecovery->setText("");
		if (_soldier->getManaMissing() > 0)
		{
			int manaRecoveryTime = _soldier->getManaRecovery(recovery.ManaRecovery);
			_txtRecovery->setText(tr("STR_MANA_RECOVERY").arg(getDaysOrInfinity(manaRecoveryTime)));
		}
		if (_soldier->getHealthMissing() > 0)
		{
			int healthRecoveryTime = _soldier->getHealthRecovery(recovery.HealthRecovery);
			_txtRecovery->setText(tr("STR_HEALTH_RECOVERY").arg(getDaysOrInfinity(healthRecoveryTime)));
		}
	}

	_txtPsionic->setVisible(_soldier->isInPsiTraining());

	//we want to apply psionic visibility at the very end to overwrite preview settings

	// Dead can't talk
	if (_base == 0)
	{
		_btnArmor->setVisible(false);
		_btnSack->setVisible(false);
		_txtCraft->setVisible(false);
		_txtDead->setVisible(true);
		std::string status = "STR_MISSING_IN_ACTION";
		if (_soldier->getDeath() && _soldier->getDeath()->getCause())
		{
			status = "STR_KILLED_IN_ACTION";
		}
		_txtDead->setText(tr(status, _soldier->getGender()));
	}
	else
	{
		_txtDead->setVisible(false);
	}
	_localChange = false; // become ready for general screen update again.
}

/**
 * Disables the soldier input.
 * @param action Pointer to an action.
 */
void SoldierInfoState::edtSoldierPress(Action *)
{
	if (_base == 0)
	{
		_edtSoldier->setFocus(false);
	}
}

/**
 * Set the soldier Id.
 */
void SoldierInfoState::setSoldierId(size_t soldier)
{
	_soldierId = soldier;
}

/**
 * Changes the soldier's name.
 * @param action Pointer to an action.
 */
void SoldierInfoState::edtSoldierChange(Action *)
{
	_soldier->setName(_edtSoldier->getText());
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void SoldierInfoState::btnOkClick(Action *)
{

	_game->popState();
	if (_game->getSavedGame()->getMonthsPassed() > -1 && Options::storageLimitsEnforced && _base != 0 && _base->storesOverfull())
	{
		_game->pushState(new SellState(_base, 0));
		_game->pushState(new ErrorMessageState(tr("STR_STORAGE_EXCEEDED").arg(_base->getName()), _palette, _game->getMod()->getInterface("soldierInfo")->getElement("errorMessage")->color, "BACK01.SCR", _game->getMod()->getInterface("soldierInfo")->getElement("errorPalette")->color));
	}
}

/**
 * Goes to the previous soldier.
 * @param action Pointer to an action.
 */
void SoldierInfoState::btnPrevClick(Action *)
{
	if (_soldierId == 0)
		_soldierId = _list->size() - 1;
	else
		_soldierId--;
	init();
}

/**
 * Goes to the next soldier.
 * @param action Pointer to an action.
 */
void SoldierInfoState::btnNextClick(Action *)
{
	_soldierId++;
	if (_soldierId >= _list->size())
		_soldierId = 0;
	init();
}

/**
 * Shows the Select Armor window.
 * @param action Pointer to an action.
 */
void SoldierInfoState::btnArmorClick(Action *)
{
	if (!_soldier->getCraft() || (_soldier->getCraft() && _soldier->getCraft()->getStatus() != "STR_OUT"))
	{
		if (_soldier->getCovertOperation() != 0)
		{
			return;
		}
		else
		{
			_game->pushState(new SoldierArmorState(_base, _soldierId, SA_GEOSCAPE));
		}
	}
}

/**
 * Shows the SoldierBonus window.
 * @param action Pointer to an action.
 */
void SoldierInfoState::btnBonusesClick(Action *)
{
	_game->pushState(new SoldierBonusState(_base, _soldierId));
}

/**
 * Shows the Sack Soldier window.
 * @param action Pointer to an action.
 */
void SoldierInfoState::btnSackClick(Action *)
{
	if (_soldier->getCovertOperation() != 0)
	{
		return;
	}
	else
	{
		_game->pushState(new SackSoldierState(_base, _soldierId));
	}
}

/**
 * Shows the Diary Soldier window.
 * @param action Pointer to an action.
 */
void SoldierInfoState::btnDiaryClick(Action *)
{
	_game->pushState(new SoldierDiaryOverviewState(_base, _soldierId, this));
}

void SoldierInfoState::cbxRolesChange(Action *action)
{
	_localChange = true;
	init();
}

/**
* Changes soldier's nationality.
* @param action Pointer to an action.
*/
void SoldierInfoState::btnFlagClick(Action *action)
{
	int temp = _soldier->getNationality();
	if (action->getDetails()->button.button == SDL_BUTTON_LEFT)
	{
		temp += 1;
	}
	else if (action->getDetails()->button.button == SDL_BUTTON_RIGHT)
	{
		temp += -1;
	}

	const std::vector<SoldierNamePool*> &names = _soldier->getRules()->getNames();
	if (!names.empty())
	{
		const int max = names.size();
		if (temp > max - 1)
		{
			temp = 0;
		}
		else if (temp < 0)
		{
			temp = max - 1;
		}
	}
	else
	{
		temp = 0;
	}

	_soldier->setNationality(temp);
	init();
}

void OpenXcom::SoldierInfoState::defineStatLines()
{
	int yPos = 80;
	int step = 11;
	if (_game->getMod()->isManaFeatureEnabled())
	{
		yPos = 81;
		step = 10;
	}
	_soldierInfoView = new SoldierInfoView(yPos, step);
}

void OpenXcom::SoldierInfoState::addStatLines()
{
	add(_soldierInfoView->_statViewTimeUnits->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewTimeUnits->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewTimeUnits->_bar, "barTUs", "soldierInfo");
	add(_soldierInfoView->_statViewManeuvering->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewManeuvering->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewManeuvering->_bar, "barManeuvering", "soldierInfo");
	add(_soldierInfoView->_statViewPhysics->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewPhysics->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewPhysics->_bar, "barPhysics", "soldierInfo");

	add(_soldierInfoView->_statViewStamina->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewStamina->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewStamina->_bar, "barEnergy", "soldierInfo");
	add(_soldierInfoView->_statViewMissiles->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewMissiles->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewMissiles->_bar, "bartMissiles", "soldierInfo");
	add(_soldierInfoView->_statViewChemistry->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewChemistry->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewChemistry->_bar, "barChemistry", "soldierInfo");

	add(_soldierInfoView->_statViewHealth->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewHealth->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewHealth->_bar, "barHealth", "soldierInfo");
	add(_soldierInfoView->_statViewDogfight->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewDogfight->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewDogfight->_bar, "barDogfight", "soldierInfo");
	add(_soldierInfoView->_statViewBiology->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewBiology->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewBiology->_bar, "barBiology", "soldierInfo");

	add(_soldierInfoView->_statViewBravery->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewBravery->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewBravery->_bar, "barBravery", "soldierInfo");
	add(_soldierInfoView->_statViewInsight->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewInsight->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewInsight->_bar, "barInsight", "soldierInfo");

	add(_soldierInfoView->_statViewReactions->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewReactions->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewReactions->_bar, "barReactions", "soldierInfo");
	add(_soldierInfoView->_statViewTracking->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewTracking->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewTracking->_bar, "barTracking", "soldierInfo");
	add(_soldierInfoView->_statViewData->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewData->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewData->_bar, "barData", "soldierInfo");

	add(_soldierInfoView->_statViewFiring->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewFiring->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewFiring->_bar, "barFiring", "soldierInfo");
	add(_soldierInfoView->_statViewTactics->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewTactics->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewTactics->_bar, "barTactics", "soldierInfo");
	add(_soldierInfoView->_statViewComputers->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewComputers->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewComputers->_bar, "barComputers", "soldierInfo");

	add(_soldierInfoView->_statViewThrowing->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewThrowing->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewThrowing->_bar, "barThrowing", "soldierInfo");
	add(_soldierInfoView->_statViewBeams->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewBeams->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewBeams->_bar, "barBeams", "soldierInfo");
	add(_soldierInfoView->_statViewMaterials->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewMaterials->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewMaterials->_bar, "barMaterials", "soldierInfo");

	add(_soldierInfoView->_statViewMelee->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewMelee->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewMelee->_bar, "barMelee", "soldierInfo");
	add(_soldierInfoView->_statViewSynaptic->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewSynaptic->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewSynaptic->_bar, "barSynaptic", "soldierInfo");
	add(_soldierInfoView->_statViewPsychology->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewPsychology->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewPsychology->_bar, "barPsychology", "soldierInfo");

	add(_soldierInfoView->_statViewStrength->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewStrength->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewStrength->_bar, "barStrength", "soldierInfo");
	add(_soldierInfoView->_statViewGravity->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewGravity->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewGravity->_bar, "barGravity", "soldierInfo");
	add(_soldierInfoView->_statViewDesigning->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewDesigning->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewDesigning->_bar, "barDesigning", "soldierInfo");

	add(_soldierInfoView->_statViewMana->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewMana->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewMana->_bar, "barMana", "soldierInfo");
	add(_soldierInfoView->_statViewPsionics->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewPsionics->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewPsionics->_bar, "barPsionics", "soldierInfo");

	add(_soldierInfoView->_statViewPsiStrength->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewPsiStrength->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewPsiStrength->_bar, "barPsiStrength", "soldierInfo");
	add(_soldierInfoView->_statViewXenolinguistics->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewXenolinguistics->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewXenolinguistics->_bar, "barXenolinguistics", "soldierInfo");

	add(_soldierInfoView->_statViewPsiSkill->_txt, "text2", "soldierInfo");
	add(_soldierInfoView->_statViewPsiSkill->_num, "numbers", "soldierInfo");
	add(_soldierInfoView->_statViewPsiSkill->_num, "barPsiSkill", "soldierInfo");
}

void OpenXcom::SoldierInfoState::nameBars()
{
	_soldierInfoView->_statViewTimeUnits->_txt->setText(tr("STR_TIME_UNITS"));
	_soldierInfoView->_statViewTimeUnits->_bar->setScale(1.0);
	_soldierInfoView->_statViewManeuvering->_txt->setText(tr("STR_MANEUVERING"));
	_soldierInfoView->_statViewManeuvering->_bar->setScale(1.0);
	_soldierInfoView->_statViewPhysics->_txt->setText(tr("STR_PHYSICS_UC"));
	_soldierInfoView->_statViewPhysics->_bar->setScale(1.0);

	_soldierInfoView->_statViewStamina->_txt->setText(tr("STR_STAMINA"));
	_soldierInfoView->_statViewStamina->_bar->setScale(1.0);
	_soldierInfoView->_statViewMissiles->_txt->setText(tr("STR_MISSILE_OPERATION"));
	_soldierInfoView->_statViewMissiles->_bar->setScale(1.0);
	_soldierInfoView->_statViewChemistry->_txt->setText(tr("STR_CHEMISTRY_UC"));
	_soldierInfoView->_statViewChemistry->_bar->setScale(1.0);

	_soldierInfoView->_statViewHealth->_txt->setText(tr("STR_HEALTH"));
	_soldierInfoView->_statViewHealth->_bar->setScale(1.0);
	_soldierInfoView->_statViewDogfight->_txt->setText(tr("STR_DOGFIGHT"));
	_soldierInfoView->_statViewDogfight->_bar->setScale(1.0);
	_soldierInfoView->_statViewBiology->_txt->setText(tr("STR_BIOLOGY_UC"));
	_soldierInfoView->_statViewBiology->_bar->setScale(1.0);

	_soldierInfoView->_statViewBravery->_txt->setText(tr("STR_BRAVERY"));
	_soldierInfoView->_statViewBravery->_bar->setScale(1.0);
	_soldierInfoView->_statViewInsight->_txt->setText(tr("STR_INSIGHT_UC"));
	_soldierInfoView->_statViewInsight->_bar->setScale(1.0);

	_soldierInfoView->_statViewReactions->_txt->setText(tr("STR_REACTIONS"));
	_soldierInfoView->_statViewReactions->_bar->setScale(1.0);
	_soldierInfoView->_statViewTracking->_txt->setText(tr("STR_TRACKING"));
	_soldierInfoView->_statViewTracking->_bar->setScale(1.0);
	_soldierInfoView->_statViewData->_txt->setText(tr("STR_DATA_ANALISYS_UC"));
	_soldierInfoView->_statViewData->_bar->setScale(1.0);

	_soldierInfoView->_statViewFiring->_txt->setText(tr("STR_FIRING_ACCURACY"));
	_soldierInfoView->_statViewFiring->_bar->setScale(1.0);
	_soldierInfoView->_statViewTactics->_txt->setText(tr("STR_TACTICS"));
	_soldierInfoView->_statViewTactics->_bar->setScale(1.0);
	_soldierInfoView->_statViewComputers->_txt->setText(tr("STR_COMPUTER_SCIENCE_UC"));
	_soldierInfoView->_statViewComputers->_bar->setScale(1.0);

	_soldierInfoView->_statViewThrowing->_txt->setText(tr("STR_THROWING_ACCURACY"));
	_soldierInfoView->_statViewThrowing->_bar->setScale(1.0);
	_soldierInfoView->_statViewBeams->_txt->setText(tr("STR_BEAMS_OPERATION"));
	_soldierInfoView->_statViewBeams->_bar->setScale(1.0);
	_soldierInfoView->_statViewMaterials->_txt->setText(tr("STR_MATERIAL_SCIENCE_UC"));
	_soldierInfoView->_statViewMaterials->_bar->setScale(1.0);

	_soldierInfoView->_statViewMelee->_txt->setText(tr("STR_MELEE_ACCURACY"));
	_soldierInfoView->_statViewMelee->_bar->setScale(1.0);
	_soldierInfoView->_statViewSynaptic->_txt->setText(tr("STR_SYNAPTIC_CONNECTIVITY"));
	_soldierInfoView->_statViewSynaptic->_bar->setScale(1.0);
	_soldierInfoView->_statViewPsychology->_txt->setText(tr("STR_PSYCHOLOGY_UC"));
	_soldierInfoView->_statViewPsychology->_bar->setScale(1.0);

	_soldierInfoView->_statViewStrength->_txt->setText(tr("STR_STRENGTH"));
	_soldierInfoView->_statViewStrength->_bar->setScale(1.0);
	_soldierInfoView->_statViewGravity->_txt->setText(tr("STR_GRAVITY_MANIPULATION"));
	_soldierInfoView->_statViewGravity->_bar->setScale(1.0);
	_soldierInfoView->_statViewDesigning->_txt->setText(tr("STR_DESIGNING_UC"));
	_soldierInfoView->_statViewDesigning->_bar->setScale(1.0);

	_soldierInfoView->_statViewMana->_txt->setText(tr("STR_MANA_POOL"));
	_soldierInfoView->_statViewMana->_bar->setScale(1.0);
	_soldierInfoView->_statViewPsionics->_txt->setText(tr("STR_PSIONICS_UC"));
	_soldierInfoView->_statViewPsionics->_bar->setScale(1.0);

	_soldierInfoView->_statViewPsiStrength->_txt->setText(tr("STR_PSIONIC_STRENGTH"));
	_soldierInfoView->_statViewPsiStrength->_bar->setScale(1.0);
	_soldierInfoView->_statViewXenolinguistics->_txt->setText(tr("STR_XENOLINGUISTICS_UC"));
	_soldierInfoView->_statViewXenolinguistics->_bar->setScale(1.0);

	_soldierInfoView->_statViewPsiSkill->_txt->setText(tr("STR_PSIONIC_SKILL"));
	_soldierInfoView->_statViewPsiSkill->_bar->setScale(1.0);
}

void SoldierInfoState::updateVisibility()
{
	_soldierInfoView->hideAllStats();
	SoldierRole selected = static_cast<SoldierRole>(_cbxRoles->getSelected());
	displayPsionic(selected);

	if (selected == ROLE_SOLDIER)
	{
		_soldierInfoView->setSoldierStatsVisible(true);
	}
	else if (selected == ROLE_PILOT)
	{
		_soldierInfoView->setPilotStatsVisible(
			true,
			_game->getSavedGame()->isResearched(_game->getMod()->getBeamOperationsUnlockResearch()),
			_game->getSavedGame()->isResearched(_game->getMod()->getCraftSynapseUnlockResearch()),
			_game->getSavedGame()->isResearched(_game->getMod()->getGravControlUnlockResearch())
		);
	}
	else if (selected == ROLE_SCIENTIST)
	{
		_soldierInfoView->setScientistStatsVisible(
			true,
			_game->getSavedGame()->isResearched(_game->getMod()->getPsiRequirements()),
			_game->getSavedGame()->isResearched(_game->getMod()->getXenologyUnlockResearch())
		);
	}
}

void OpenXcom::SoldierInfoState::displayPsionic(SoldierRole selected)
{
	if (_game->getMod()->isManaFeatureEnabled()
		&& _game->getSavedGame()->isManaUnlocked(_game->getMod())
		&& (selected == ROLE_SOLDIER || selected == ROLE_AGENT)) 
	{
		_soldierInfoView->_statViewMana->setVisible(true);
	}

	bool psi = _soldier->getStatsWithSoldierBonusesOnly()->psiSkill > 0
		&& (selected == ROLE_SOLDIER || selected == ROLE_AGENT || selected == ROLE_PILOT);
	if (psi || ((Options::psiStrengthEval && !_ftaUI) && _game->getSavedGame()->isResearched(_game->getMod()->getPsiRequirements())))
	{
		_soldierInfoView->_statViewPsiStrength->setVisible(true);
	}
	if (psi)
	{
		_soldierInfoView->_statViewPsiSkill->setVisible(true);
	}
}

}
