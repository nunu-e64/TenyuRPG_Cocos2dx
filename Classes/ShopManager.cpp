#include "ShopManager.h"
#include "Item.h"


void CShopManager::Init() {

	ShopMenu = &ShopMenuInstance;
	ShopBank.clear();
	CurrentOpenShopIndex = -1;

}

bool CShopManager::AddShop(int _index, std::vector<std::string> _itemList) {

	if (ShopBank.find(_index) == ShopBank.end()) {

		ShopBank[_index] = _itemList;
		return true;

	} else {
		ERRORDX("Already Exist Key:%d (do nothing)", _index);
		return false;
	}
}

std::vector <std::string> CShopManager::GetShop(int _index) {

	if (ShopBank.find(_index) != ShopBank.end()) {
		return ShopBank[_index];
	
	} else {
		ERRORDX("Not Found Shop: Key=%d (return BlankList)", _index);
		return std::vector<std::string>();
	}
}


//開店
bool CShopManager::OpenShop(int _index) {

	if (ShopBank.find(_index) != ShopBank.end()) {

		CItemManager* itemManager = CItemManager::GetInstance();
		ShopMenu->ItemManager = itemManager;

		CurrentOpenShopIndex = _index;
		ShopMenu->Cursor = 0;
		ShopMenu->IsConfirm = false;

		ShopMenu->ItemList.clear();
		ShopMenu->Basket.clear();
		for (unsigned int i = 0; i < ShopBank[CurrentOpenShopIndex].size(); i++) {
			ShopMenu->ItemList.push_back(itemManager->GetItem(ShopBank[CurrentOpenShopIndex][i]));
			ShopMenu->Basket.push_back(0);
		}

		//合計金額
		ShopMenuInstance.SumPrice = 0;

		return true;

	} else {
		ERRORDX("Not Found Shop: Key=%d (return BlankList)", _index);
		return false;
	}

}


bool CShopManager::IsOpen() {
	return (CurrentOpenShopIndex >= 0);
}


bool CShopManager::Main() {

	if (!IsOpen()) return false;

	//カーソル移動
	if (CheckHitKeyDown(KEY_INPUT_DOWN)) {
		ShopMenu->Move(DOWN);
	
	} else if (CheckHitKeyDown(KEY_INPUT_UP)) {
		ShopMenu->Move(UP);
	
	}else if (CheckHitKeyDown(KEY_INPUT_RIGHT)) {
		ShopMenu->Move(RIGHT);
	
	} else if (CheckHitKeyDown(KEY_INPUT_LEFT)) {
		ShopMenu->Move(LEFT);
	
	} else if (CheckHitKeyDown(KEY_INPUT_OK)) {
		if (ShopMenu->IsConfirm) {
			ShopMenu->Buy();		//購入
			ShopMenu->IsConfirm = false;
		} else {
			if (ShopMenu->CanBuy()) {
				ShopMenu->IsConfirm = true;
			}
		}
	
	} else if (CheckHitKeyDown(KEY_INPUT_CANCEL)) {

		if (ShopMenu->IsConfirm) {
			ShopMenu->IsConfirm = false;
		} else if (ShopMenu->CanClose()){
			CurrentOpenShopIndex = -1;	//閉店
		} else {
			OpenShop(CurrentOpenShopIndex);	//バスケットリセット
		}
	}

	return IsOpen();
}


void CShopManager::Draw() {
	if (!IsOpen()) return;

	ShopMenu->Draw();
}

void CShopMenu::Move(int _dir) {

	if (!IsConfirm) {

		switch (_dir) {
		case direction_tag::DOWN:
			Cursor = mod(Cursor + 1, ItemList.size());
			break;

		case direction_tag::UP:
			Cursor = mod(Cursor - 1, ItemList.size());
			break;

		case direction_tag::RIGHT:
			if (Cursor < (int)ItemList.size()) {

				if (SumPrice + ItemList[Cursor]->Price <= ItemManager->GetGold()) {

					Basket[Cursor] =
						min(ItemList[Cursor]->OwnLimit - ItemManager->GetPlayerItemNum(ItemList[Cursor]->Name)
							, Basket[Cursor] + 1);
				}
			}
			break;

		case direction_tag::LEFT:
			Basket[Cursor] = max(0, Basket[Cursor] - 1);
			break;
		}

		//合計金額計算
		SumPrice = 0;
		for (unsigned int i = 0; i < ItemList.size(); i++) {
			SumPrice += ItemList[i]->Price * Basket[i];
		}


	}

}

bool CShopMenu::CanBuy(){
	if (SumPrice <= ItemManager->GetGold()) {
		return true;
	} else {
		return false;
	}
}

bool CShopMenu::CanClose(){
	if (SumPrice == 0) {
		return true;
	} else {
		return false;
	}
}

bool CShopMenu::Buy() {

	ItemManager->DecGold(SumPrice);
	for (unsigned int i = 0; i < ItemList.size(); i++) {
		ItemManager->IncPlayerItem(ItemList[i]->Name, Basket[i]);
		Basket[i] = 0;	//バスケットは空にする
	}
	SumPrice = 0;

	return true;

}

void CShopMenu::Draw() {

	CRect rect(60, WINDOW_WIDTH - 60, 60, WINDOW_HEIGHT - 60);

	DrawBox(rect, GetColor(30, 20, 80), true);

	//商品リスト
	std::string strNum;
	for (unsigned int i = 0; i < ItemList.size(); i++) {
		int top = rect.Top + 20 + i*(2 + GetFontSize());

		if (Cursor == i && !IsConfirm) {
			DrawString(rect.Left + 20, top, "|>", WHITE, BLACK);	//カーソル
		}

		//商品名
		DrawString(rect.Left + 50, top, ItemList[i]->Name.c_str(), WHITE, BLACK);

		//商品金額
		strNum = std::to_string(ItemList[i]->Price) + "ガル";
		DrawString(rect.Right - 240, top, strNum.c_str(), WHITE, BLACK);

		//購入個数
		strNum = (Basket[i]>0 ? "< " : "  ") + std::to_string(Basket[i])
			+ (Basket[i] < ItemList[i]->OwnLimit - ItemManager->GetPlayerItemNum(ItemList[i]->Name) && SumPrice+ItemList[i]->Price <= ItemManager->GetGold() ? " >" : "  ");
		DrawString(rect.Right - 120, top, strNum.c_str(), WHITE, BLACK);

		//所持個数と所持上限
		strNum = std::to_string(ItemManager->GetPlayerItemNum(ItemList[i]->Name)) + "/" + std::to_string(ItemList[i]->OwnLimit);
		DrawString(rect.Right - 60, top, strNum.c_str(), WHITE, BLACK);
	}

	//決定
	int top = rect.Bottom - 40;
	if (IsConfirm) {
		DrawString(rect.Left + 30, top, "|>", WHITE, BLACK);
	}
	DrawString(rect.Left + 60, top, "決定", (IsConfirm ? WHITE : GRAY), BLACK);

	//合計金額と所持金額
	strNum = "計：" + std::to_string(SumPrice) + "ガル  所持金：" + std::to_string(ItemManager->GetGold()) + "ガル";
	DrawString(rect.Right - 250, top, strNum.c_str(), WHITE, BLACK);

}