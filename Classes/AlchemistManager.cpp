#include "AlchemistManager.h"
#include "Item.h"

void CAlchemistManager::Init() {

	ShopMenu = &AlchemistMenuInstance;
	ShopBank.clear();
	CurrentOpenShopIndex = -1;

}

//開店
bool CAlchemistManager::OpenShop(int _index) {

	if (ShopBank.find(_index) != ShopBank.end()) {

		CItemManager* itemManager = CItemManager::GetInstance();
		ShopMenu->ItemManager = itemManager;

		CurrentOpenShopIndex = _index;
		ShopMenu->Cursor = 0;
		ShopMenu->IsConfirm = false;

		ShopMenu->ItemList.clear();
		ShopMenu->Basket.clear();
		for (unsigned int i = 0; i < ShopBank[CurrentOpenShopIndex].size(); i++) {
			CAccessoryItem* accessoryItem;
			accessoryItem = itemManager->GetAccessoryItem(ShopBank[CurrentOpenShopIndex][i]);

			ShopMenu->ItemList.push_back(accessoryItem);
			AlchemistMenuInstance.CurrentMaterialSet.push_back(accessoryItem->MaterialSet);
		}

		return true;

	} else {
		ERRORDX("Not Found Shop: Key=%d (return BlankList)", _index);
		return false;
	}

}


void CAlchemistMenu::Move(int _dir) {

	if (!IsConfirm) {

		switch (_dir) {
		case direction_tag::DOWN:
			Cursor = mod(Cursor + 1, ItemList.size());
			break;

		case direction_tag::UP:
			Cursor = mod(Cursor - 1, ItemList.size());
			break;

		case direction_tag::RIGHT:
		case direction_tag::LEFT:
			break;
		}
	}

}

bool CAlchemistMenu::Buy() {

	for (unsigned int i = 0; i < CurrentMaterialSet[Cursor].size(); i++) {
		auto tmpPair = CurrentMaterialSet[Cursor][i];
		ItemManager->DecPlayerItem(tmpPair.first, tmpPair.second);
	}

	ItemManager->IncPlayerItem(ItemList[Cursor]->Name, 1);

	return true;
}

bool CAlchemistMenu::CanBuy() {

	for (unsigned int i = 0; i < CurrentMaterialSet[Cursor].size(); i++) {
		auto tmpPair = CurrentMaterialSet[Cursor][i];

		if (ItemManager->GetPlayerItemNum(tmpPair.first) < tmpPair.second) {
			return false;
		}
	}

	return true;
}

bool CAlchemistMenu::CanClose() {
	return true;
}

void CAlchemistMenu::Draw() {

	CRect rect(60, WINDOW_WIDTH - 60, 60, WINDOW_HEIGHT - 60);
	CRect rectMaterial(WINDOW_WIDTH/2 + 20, rect.Right-20, rect.Top+20, rect.Bottom-20);

	DrawBox(rect, GetColor(30, 20, 80), true);
	DrawBox(rectMaterial, GetColor(80, 20, 30), true);

	//商品リスト
	std::string strNum;
	for (unsigned int i = 0; i < ItemList.size(); i++) {
		int top = rect.Top + 20 + i*(2 + GetFontSize());

		if (Cursor == i && !IsConfirm) {
			DrawString(rect.Left + 20, top, "|>", WHITE, BLACK);	//カーソル
		}

		//装備アイテム名
		DrawString(rect.Left + 50, top, ItemList[i]->Name.c_str(), WHITE, BLACK);

		//現在の所持数と所持上限
		strNum = std::to_string(ItemManager->GetPlayerItemNum(ItemList[i]->Name)) + "/" + std::to_string(ItemList[i]->OwnLimit);
		DrawString(rect.Left + 220, top, strNum.c_str(), WHITE, BLACK);
	}

	for (unsigned int i = 0; i < CurrentMaterialSet[Cursor].size(); i++) {

		auto tmpPair = CurrentMaterialSet[Cursor][i];
		int top = rectMaterial.Top + 20 + i*(2 + GetFontSize());
	
		//錬成必要素材名
		DrawString(rectMaterial.Left + 20, top, tmpPair.first.c_str(), WHITE, BLACK);

		//錬成必要素材個数と所持個数。不足なら赤文字
		strNum = std::to_string(ItemManager->GetPlayerItemNum(tmpPair.first)) + "/"+ std::to_string(tmpPair.second);
		int color = (tmpPair.second > ItemManager->GetPlayerItemNum(tmpPair.first) ? RED : WHITE);
		DrawString(rectMaterial.Right - 60, top, strNum.c_str(), color, BLACK);

	}

	//決定
	int top = rect.Bottom - 40;
	if (IsConfirm) {
		DrawString(rect.Left + 30, top, "|>", WHITE, BLACK);
	}
	DrawString(rect.Left + 60, top, "決定", (IsConfirm ? WHITE : GRAY), BLACK);
	
}