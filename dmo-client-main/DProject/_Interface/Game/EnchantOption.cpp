#include "stdafx.h"
#include "EnchantOption.h"

#define IF_ACCESSORY_PROGRESS_TIME 1.667f
#define IF_ACCESSORY_ENCHANT_WND_WIDTH 709
#define IF_ACCESSORY_ENCHANT_WND_HEIGHT 439
#define IF_ACCESSORY_ENCHANT_LEFT_WIDTH 397

cEnchantOption::cEnchantOption() :
m_nUIState( 0 ),
m_pTimeProgress( NULL ),
m_pRegistStone( NULL ),
m_pRegistItem( NULL ),
m_pOptValCombo( NULL ),
m_pEnchantBtn( NULL ),
m_pCancelBtn( NULL ),
m_pEffectRender( NULL ),
m_pEffectObj( NULL ),
m_pItemTypeName( NULL ),
m_pStoneTypeName( NULL ),
m_pStoneCountText( NULL ),
m_pSameOptionText( NULL ),
m_pAvailableOptionList( NULL )
{
	for( int i = 0; i < nLimit::MAX_ACCESSORY_OPTION; ++i )
	{
		m_pCurrentOptionText[ i ] = NULL;
		m_pCurrentOptionMaxText[ i ] = NULL;
		m_pCurrentOptionLockBtn[ i ] = NULL;
	}
}

cEnchantOption::~cEnchantOption()
{

}

bool cEnchantOption::Construct(void)
{
	if( !CONTENTSSYSTEM_PTR )
		return false;

	SetSystem( CONTENTSSYSTEM_PTR->GetContents< SystemType >( SystemType::IsContentsIdentity() ) );

	if( !GetSystem() )
		return false;

	GetSystem()->Register( SystemType::eIdentifyItem,		this );
	GetSystem()->Register( SystemType::eRegistEnchantItem,	this );
	GetSystem()->Register( SystemType::eRegistEnchantStone,	this );
	GetSystem()->Register( SystemType::eStartEnchant,		this );
	GetSystem()->Register( SystemType::eSuccessEnchant,		this );

	return true;
}

void cEnchantOption::Notify(int const& iNotifiedEvt, ContentsStream const& kStream)
{
	switch( iNotifiedEvt )
	{
	case SystemType::eIdentifyItem:
		{
			if( m_pTimeProgress )
				m_pTimeProgress->Start();
		}
		break;
	case SystemType::eRegistEnchantItem:
		{
			uint nItemType = 0;
			kStream >> nItemType;
			_RegistEnchantItem( nItemType );
		}
		break;
	case SystemType::eRegistEnchantStone:
		{
			uint nItemType = 0;
			uint nEnchantType = 0;
			kStream >> nItemType >> nEnchantType;
			_RegistEnchantStone( nItemType, nEnchantType );
		}
		break;
	case SystemType::eStartEnchant:
		_StartEnchant();
		break;
	case SystemType::eSuccessEnchant:
		_SuccessEnchant();
		break;
	}
}

cBaseWindow::eWINDOW_TYPE cEnchantOption::GetWindowType()
{
	return WT_ENCHANT_OPTION;
}

void cEnchantOption::Destroy()
{
	if( GetSystem() )
		GetSystem()->UnRegisterAll( this );

	cBaseWindow::Delete();
}

void cEnchantOption::DeleteResource()
{
	DeleteScript();

	if( m_pEffectRender )
		SAFE_NIDELETE( m_pEffectRender );
}

void cEnchantOption::Create(int nValue /*= 0 */)
{
	if( !cBaseWindow::Init() )
		return;

	m_nUIState = GetSystem()->GetUIState();
	switch( m_nUIState )
	{
	case cEnchantOptionContents::eEnchantUI_Enchant:
		_CreateEnchantUI();
		break;
	case cEnchantOptionContents::eEnchantUI_Identify:
		_CreateIdentifyUI();
		break;
	case cEnchantOptionContents::eEnchantUI_None:
		return;
	}
}

void cEnchantOption::Update(float const& fDeltaTime)
{
	UpdateScript( fDeltaTime );

	_UpdateMoveWindow();
}

cBaseWindow::eMU_TYPE cEnchantOption::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( MUT_OUT_WINDOW == muReturn )
		return muReturn;

	if( m_pRegistStone && m_pRegistStone->Update_ForMouse( CURSOR_ST.GetPos() ) )
	{
		_SetTooltipEnchantStone( m_pRegistStone->GetMouseOverItem() );
		return muReturn;
	}

	if( m_pRegistItem && m_pRegistItem->Update_ForMouse( CURSOR_ST.GetPos() ) )
	{
		_SetTooltipEnchantItem( m_pRegistItem->GetMouseOverItem() );
		return muReturn;
	}

	if( m_pOptValCombo && m_pOptValCombo->Update_ForMouse() != cComboBox::CI_INVALIDE )
		return muReturn;

	for( int i = 0; i < nLimit::MAX_ACCESSORY_OPTION; ++i )
	{
		if( m_pCurrentOptionLockBtn[ i ] && m_pCurrentOptionLockBtn[ i ]->Update_ForMouse() == cSprite::ACTION_CLICK )
		{
			_ToggleOptionLock( i );
			return muReturn;
		}
	}

	if( m_pAvailableOptionList && m_pAvailableOptionList->Update_ForMouse( CURSOR_ST.GetPos() ) )
		return muReturn;

	if( m_pEnchantBtn && m_pEnchantBtn->Update_ForMouse() == cButton::ACTION_CLICK )
		return muReturn;

	if( m_pCancelBtn && m_pCancelBtn->Update_ForMouse() == cButton::ACTION_CLICK )
		return muReturn;

	if( CURSOR_ST.CheckClickBox( CsRect( GetRootClient(), CsSIZE( 300, 36 ) ) ) != CURSOR::BUTTON_OUTWINDOW )
		_UpdateMoveWindow_ForMouse();

	return muReturn;
}

void cEnchantOption::Render()
{
	RenderScript();

	if( m_pEffectRender && m_pEffectObj )
	{
		m_pEffectRender->BeginRender();
		m_pEffectRender->RenderEffect( GET_SUBCAMERA(CAMERA_14), m_pEffectObj, 260.0f );
		m_pEffectRender->EndRender( GET_SUBCAMERA(CAMERA_14), CsPoint( -170, -320 ) + GetRootClient() );
	}
}

void cEnchantOption::ResetDevice()
{
	ResetDeviceScript();

	if( m_pEffectRender )
		m_pEffectRender->ResetDevice();
}

bool cEnchantOption::OnEscapeKey()
{
	if( cEnchantOptionContents::eEnchantUI_Identify == m_nUIState )
		return false;

	GetSystem()->CloseEnchant();
	return true;
}

bool cEnchantOption::CursorIconLBtnUp(CURSOR_ICON::eTYPE eIconType, int nIconSlot, int nIconCount)
{
	if( cEnchantOptionContents::eEnchantUI_Identify == m_nUIState )
		return false;

	switch( eIconType )
	{
	case CURSOR_ICON::CI_INVEN:
		{
			if( !GetSystem()->IsEnableRegist( nIconSlot, nIconCount ) )
				return false;

			GetSystem()->RegistEnchantItem( nIconSlot );
			CURSOR_ST.ReleaseIcon();
			return true;
		}
		break;
	}

	return false;
}

void cEnchantOption::_CreateIdentifyUI()
{
	CsPoint ptSize( 192, 62 );
	CsPoint ptPos( (g_nScreenWidth - ptSize.x) / 2, (g_nScreenHeight - ptSize.y) * 0.8f );

	InitScript( NULL, ptPos, ptSize, false );

	{	// 감정중 . . .
		cText::sTEXTINFO ti;
		ti.Init( CFont::FS_13, FONT_RED );
		ti.s_eTextAlign = DT_CENTER;
		ti.SetText( UISTRING_TEXT( "ACCESSORY_IDENTIFY_IDENTIFYING" ).c_str() );
		AddText( &ti, CsPoint( ptSize.x / 2, 0 ) );
	}

	m_pTimeProgress = NiNew cUI_TimerProgressBar;
	if( m_pTimeProgress )
	{
		m_pTimeProgress->Init( m_pRoot, CsPoint( 0, 20 ), CsPoint( 192, 32 ), false );
		m_pTimeProgress->AddEvent( cUI_ProgressBar::eProgressbar_Done, this, &cEnchantOption::_IdentifyProcessEnd );
		m_pTimeProgress->SetBackImage( "Accessory\\Identify\\Identify_BarBG.tga" );
		m_pTimeProgress->SetGuageImage( "Accessory\\Identify\\Identify_Bar.tga" );
		m_pTimeProgress->SetCoverImage( "Accessory\\Identify\\Identify_BarMask.tga" );
		m_pTimeProgress->SetViewTextType( cUI_ProgressBar::NONE );
		m_pTimeProgress->SetProgressRect( CsRect( CsPoint( 6, 6 ), CsSIZE( 180, 22 ) ) );
		m_pTimeProgress->SetStepSize( 0.01f );
		m_pTimeProgress->SetTimer( 0.0f, IF_ACCESSORY_PROGRESS_TIME );
		m_pTimeProgress->Stop();
		AddChildControl( m_pTimeProgress );
	}

	cWindow::PlaySound( "System\\System_return_01.wav" );
}

void cEnchantOption::_CreateEnchantUI()
{
	CsPoint ptSize( IF_ACCESSORY_ENCHANT_WND_WIDTH, IF_ACCESSORY_ENCHANT_WND_HEIGHT );
	CsPoint ptPos( (g_nScreenWidth - ptSize.x) / 2, (g_nScreenHeight - ptSize.y) / 2 );

	InitScript( NULL, ptPos, ptSize, true );
	AddSprite( CsPoint::ZERO, ptSize, "Accessory\\Enchant\\Enchant_BG1.tga", true );
	AddTitle( UISTRING_TEXT( "ACCESSORY_ENCHANT_TITLE_ACC_ENCHANT" ).c_str(), CsPoint( 0, 5 ) );

	{	// 스톤
		cText::sTEXTINFO ti;
		ti.Init( CFont::FS_12, NiColor::WHITE );
		ti.s_eTextAlign = DT_CENTER;
		ti.SetText( L"Stone" );
		m_pStoneTypeName = AddText( &ti, CsPoint( 264, 58 ) );
	}

	{	// 악세서리
		cText::sTEXTINFO ti;
		ti.Init( CFont::FS_12, NiColor::WHITE );
		ti.s_eTextAlign = DT_CENTER;
		ti.SetText( UISTRING_TEXT( "COMMON_TXT_ACCESSORY" ).c_str() );
		m_pItemTypeName = AddText( &ti, CsPoint( 154, 58 ) );
	}

	{
		cText::sTEXTINFO ti;
		ti.Init( CFont::FS_10, NiColor( 0.45f, 0.75f, 1.0f ) );
		ti.s_eTextAlign = DT_LEFT;
		ti.SetText( L"" );
		m_pStoneCountText = AddText( &ti, CsPoint( 291, 92 ) );
	}

	m_pTimeProgress = NiNew cUI_TimerProgressBar;
	if( m_pTimeProgress )
	{
		m_pTimeProgress->Init( m_pRoot, CsPoint::ZERO, ptSize, false );
		m_pTimeProgress->AddEvent( cUI_ProgressBar::eProgressbar_Done, this, &cEnchantOption::_EnchantProcessEnd );
		m_pTimeProgress->SetViewTextType( cUI_ProgressBar::NONE );
		m_pTimeProgress->SetTimer( 0.0f, IF_ACCESSORY_PROGRESS_TIME );
		m_pTimeProgress->Stop();
		AddChildControl( m_pTimeProgress );
	}

	CsPoint ptBtnSize( 101, 33 );
	CsPoint ptBtnPos( 158, 128 );
	m_pEnchantBtn = AddButton( ptBtnPos, ptBtnSize, CsPoint( 0, 26 ), "Accessory\\Enchant\\Enchant_EnchantBtn.tga" );
	if( m_pEnchantBtn )
	{
		cText::sTEXTINFO ti;
		ti.Init( CFont::FS_10, NiColor::WHITE );
		ti.s_eTextAlign = DT_CENTER;
		ti.SetText( UISTRING_TEXT( "ACCESSORY_ENCHANT_ENCHANTMENT" ).c_str() );
		m_pEnchantBtn->SetText( &ti );
		m_pEnchantBtn->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cEnchantOption::_OnClickEnchant );
		m_pEnchantBtn->SetEnable( false );
	}

	m_pCancelBtn = AddButton( CsPoint( 680, 16 ), CsPoint( 20, 20 ), CsPoint( 0, 16 ), "System\\Ch_Close.tga" );
	if( m_pCancelBtn )
		m_pCancelBtn->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cEnchantOption::_OnClickClose );

	_MakeEnchantItem();
	_MakeAccessoryItem();
	_MakeOptionValueBox();
	_MakeOptionPanels();
	_RefreshOptionPanels();
}

void cEnchantOption::_MakeEnchantItem()
{
	m_pRegistStone = NiNew cListBox;
	SAFE_POINTER_RET( m_pRegistStone );

	m_pRegistStone->Init( m_pRoot, CsPoint( 243, 72 ), CsPoint( 42, 42 ), NULL, false );
	m_pRegistStone->SetBackOverAndSelectedImgRender( false );
	m_pRegistStone->AddEvent( cListBox::LIST_ITEM_RIGHT_UP, this, &cEnchantOption::_OnRClickEnchantStone );
	AddChildControl( m_pRegistStone );

	m_pRegistStone->RemoveAllItem();

	cString* pControl = NiNew cString;
	SAFE_POINTER_RET( pControl );

	uint nItemType = GetSystem()->GetEnchantStoneItemType();
	m_sEnchantStone.bIsRegist = ( nItemType == 0 ) ? false : true;
	m_sEnchantStone.pItemIcon = pControl->AddIcon( CsPoint( 32, 32 ), ICONITEM::ITEM_ICON, nItemType, 1, CsPoint( 5, 5 ) );
	if( m_sEnchantStone.pItemIcon )
		m_sEnchantStone.pItemIcon->SetAutoPointerDelete( true );

	cListBoxItem* pLBItem = NiNew cListBoxItem;
	if( pLBItem )
	{
		pLBItem->SetItem( pControl );
		m_pRegistStone->AddItem( pLBItem );
	}
}

void cEnchantOption::_MakeAccessoryItem()
{
	m_pRegistItem = NiNew cListBox;
	SAFE_POINTER_RET( m_pRegistItem );

	m_pRegistItem->Init( m_pRoot, CsPoint( 133, 72 ), CsPoint( 42, 42 ), NULL, false );
	m_pRegistItem->SetBackOverAndSelectedImgRender( false );
	m_pRegistItem->AddEvent( cListBox::LIST_ITEM_RIGHT_UP, this, &cEnchantOption::_OnRClickEnchantItem );
	AddChildControl( m_pRegistItem );

	m_pRegistItem->RemoveAllItem();

	cString* pControl = NiNew cString;
	SAFE_POINTER_RET( pControl );

	uint nItemType = GetSystem()->GetEnchantItemType();
	m_sEnchantItem.bIsRegist = ( nItemType == 0 ) ? false : true;
	m_sEnchantItem.pItemIcon = pControl->AddIcon( CsPoint( 32, 32 ), ICONITEM::ITEM_ICON, nItemType, 1, CsPoint( 5, 5 ) );
	if( m_sEnchantItem.pItemIcon )
		m_sEnchantItem.pItemIcon->SetAutoPointerDelete( true );

	cListBoxItem* pLBItem = NiNew cListBoxItem;
	if( pLBItem )
	{
		pLBItem->SetItem( pControl );
		m_pRegistItem->AddItem( pLBItem );
	}
}

void cEnchantOption::_IdentifyProcessEnd(void* pSender, void* pData)
{
	GetSystem()->SuccessIdentifyItem();
}

void cEnchantOption::_RegistEnchantStone(uint nItemType, uint nEnchantType)
{
	cWindow::PlaySound( "System\\interface_person_01.wav" );

	m_sEnchantStone.ChangeItem( nItemType );
	_RefreshStoneCountText();

	if( m_pTimeProgress )
	{
		m_pTimeProgress->SetTimer( 0.0f, IF_ACCESSORY_PROGRESS_TIME );
		m_pTimeProgress->Stop();
	}

	if( !m_sEnchantItem.bIsRegist )
		return;

	if( nItem::OptionValue == nEnchantType )
		_SetOptionValueBox();
	else
	{
		if( m_pOptValCombo )
			m_pOptValCombo->SetVisible( false );
	}

	_SetEnchantButton();
	_RefreshOptionPanels();
}

void cEnchantOption::_RegistEnchantItem(uint nItemType)
{
	cWindow::PlaySound( "System\\interface_person_01.wav" );

	m_sEnchantItem.ChangeItem( nItemType );
#ifdef LJW_ENCHANT_OPTION_DIGIVICE_190904
	if( m_pItemTypeName )
		m_pItemTypeName->SetText( GetSystem()->GetEnchantItemTypeName().c_str() );
#endif

	uint nEnchantType = GetSystem()->GetEnchantStoneEnchantType();
	if( nItem::OptionValue == nEnchantType )
		_SetOptionValueBox();

	_SetEnchantButton();
	_RefreshOptionPanels();
}

void cEnchantOption::_StartEnchant()
{
	if( m_pEnchantBtn )
		m_pEnchantBtn->SetEnable( false );

	if( !m_pEffectRender )
		m_pEffectRender = NiNew cRenderTex;
	if( !m_pEffectRender->Init( CsPoint( 660, 660 ) ) )
		SAFE_NIDELETE( m_pEffectRender );

	m_pEffectObj = GetSystem()->GetEnchantItemEffect();

	if( m_pTimeProgress )
		m_pTimeProgress->Start();

	m_sEnchantStone.SetVisible( false );
	m_sEnchantItem.SetVisible( false );

	cWindow::PlaySound( "System\\JewelSuccess.wav" );
}

void cEnchantOption::_SuccessEnchant()
{
	m_sEnchantStone.ChangeItem( GetSystem()->GetEnchantStoneItemType() );
	_RefreshStoneCountText();

	m_pEffectObj = NULL;

	uint nEnchantType = GetSystem()->GetEnchantStoneEnchantType();
	if( nItem::OptionValue == nEnchantType )
		_SetOptionValueBox();
	else if( m_pOptValCombo )
		m_pOptValCombo->SetVisible( false );

	_SetEnchantButton();
	_RefreshOptionPanels();
}

void cEnchantOption::_MakeOptionValueBox()
{
	m_pOptValCombo = NiNew cComboBox;
	if( m_pOptValCombo )
	{
#ifdef LJW_ENCHANT_OPTION_DIGIVICE_190904
		m_pOptValCombo->Init( m_pRoot, cComboBox::CT_NORMAL2, CsPoint( 71, 150 ), CsPoint( 180, 25 ),
			CFont::FS_10, DT_LEFT, false, false, false );
#else
		m_pOptValCombo->Init( m_pRoot, cComboBox::CT_NORMAL2, CsPoint( 41, 150 ), CsPoint( 130, 20 ),
			CFont::FS_10, DT_LEFT, false, false, false );
#endif
		m_pOptValCombo->AddEvent( cComboBox::COMBO_SELECTITEM, this, &cEnchantOption::_OnClickComboBox );
		m_pOptValCombo->SetComboListWindow( "SealMaster\\dropdown_list_0" );
		m_pOptValCombo->SetDeltaPos( CsPoint( 10, 0 ) );
		m_pOptValCombo->SetVisible( false );
		AddChildControl( m_pOptValCombo );
	}
}

void cEnchantOption::_SetOptionValueBox()
{
	SAFE_POINTER_RET( m_pOptValCombo );
	m_pOptValCombo->RemoveAllItem();
	m_pOptValCombo->SetVisible( true );

	const cItemInfo* pAccItem = GetSystem()->GetEnchantItem();
	SAFE_POINTER_RET( pAccItem );

	for( int i = 0; i < nLimit::MAX_ACCESSORY_OPTION; ++i )
	{
		if( pAccItem->m_nAccOption[ i ] == 0 )
			continue;

		_AddOptionValueItem( pAccItem->m_nAccOption[ i ], pAccItem->m_nAccValues[ i ], i );
	}

	m_pOptValCombo->SetCurSel( 0 );
	int nAccIndex = m_pOptValCombo->GetData( 0 );
	GetSystem()->SelectOptionValue( nAccIndex );
}

void cEnchantOption::_AddOptionValueItem(ushort nOption, ushort nValue, uint nIndex)
{
	SAFE_POINTER_RET( m_pOptValCombo );
	std::wstring wsMsg = _GetAccOptionValue( nOption, static_cast<float>(nValue) );
	m_pOptValCombo->AddItem( wsMsg.c_str(), nIndex );
}

std::wstring cEnchantOption::_GetAccOptionValue(ushort nOption, float fValue) const
{
	nOption = static_cast<ushort>( nItem::NormalizeAccessoryOption( nOption ) );
	std::wstring wsText;
	switch( nOption )
	{
	case nItem::AP:
		DmCS::StringFn::Format( wsText, L"%s + %d", UISTRING_TEXT( "COMMON_TXT_ATTACK" ).c_str(), static_cast<int>(fValue) );
		break;
	case nItem::DP:
		DmCS::StringFn::Format( wsText, L"%s + %d", UISTRING_TEXT( "COMMON_TXT_DEFENSE" ).c_str(), static_cast<int>(fValue) );
		break;
	case nItem::MAXHP:
		DmCS::StringFn::Format( wsText, L"%s + %d", UISTRING_TEXT( "COMMON_TXT_MAX_HP" ).c_str(), static_cast<int>(fValue) );
		break;
	case nItem::MAXDS:
		DmCS::StringFn::Format( wsText, L"%s + %d", UISTRING_TEXT( "COMMON_TXT_MAX_DS" ).c_str(), static_cast<int>(fValue) );
		break;
	case nItem::SkillAP:
		DmCS::StringFn::Format( wsText, L"%s + %d", UISTRING_TEXT( "COMMON_TXT_SKILLAP" ).c_str(), static_cast<int>(fValue) );
		break;
	case nItem::AttrAP:
		DmCS::StringFn::Format( wsText, L"%s + %.2f", UISTRING_TEXT( "COMMON_TXT_ATTRIBUTEAP" ).c_str(), fValue );
		break;
	case nItem::CA:
		DmCS::StringFn::Format( wsText, L"%s + %.2f", UISTRING_TEXT( "COMMON_TXT_CRITICAL" ).c_str(), fValue );
		break;
	case nItem::CD:	//치명타데미지증가
		DmCS::StringFn::Format( wsText, L"%s + %5.2f", UISTRING_TEXT( "COMMON_TXT_CRITICAL_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AS:	// 공속증가
		DmCS::StringFn::Format( wsText, L"%s + %5.2f", UISTRING_TEXT( "COMMON_TXT_ATTACK_SPEED" ).c_str(), fValue / 100.0f );
		break;
	case nItem::EV:	// 회피
		DmCS::StringFn::Format( wsText, L"%s + %.2f", UISTRING_TEXT( "COMMON_TXT_AVOID" ).c_str(), fValue );
		break;	
	case nItem::BL:	// 블럭
		DmCS::StringFn::Format( wsText, L"%s + %.2f", UISTRING_TEXT( "COMMON_TXT_BLOCK" ).c_str(), fValue );
		break;
	case nItem::HT:	// 적중도
		DmCS::StringFn::Format( wsText, L"%s + %d", UISTRING_TEXT( "COMMON_TXT_HIT_RATE" ).c_str(), static_cast<int>(fValue) );
		break;

	case nItem::AP_RATIO:
	case nItem::DP_RATIO:
	case nItem::MAXHP_RATIO:
	case nItem::MAXDS_RATIO:
	case nItem::SkillAP_RATIO:
	case nItem::FINAL_AP_RATIO:
		DmCS::StringFn::Format( wsText, L"%s + %5.2f", nItem::GetAccessoryRatioOptionText( nOption ), fValue / 100.0f );
		break;

#if 1
		// 속성 데미지 증가
	case nItem::AP_ATTRIBUTE_DA:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_DATA_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_VA:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_VACCINE_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_VI:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_VIRUS_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_UN:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_UNKNOWN_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_IC:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_ICE_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_WA:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_WATER_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_FI:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_FIRE_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_EA:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_EARTH_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_WI:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_WIND_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_WO:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_WOOD_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_LI:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_LIGHT_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_DK:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_DARKNESS_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_TH:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_THUNDER_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
	case nItem::AP_ATTRIBUTE_ST:
		DmCS::StringFn::Format( wsText, L" %s + %5.2f", UISTRING_TEXT( "COMMON_TXT_STEEL_ATTRIBUTE_DAMAGE" ).c_str(), fValue / 100.0f );
		break;
#endif
	}

	return wsText;
}

void cEnchantOption::_MakeOptionPanels()
{
	{
		cText::sTEXTINFO ti;
		ti.Init( CFont::FS_12, NiColor::WHITE );
		ti.s_eTextAlign = DT_CENTER;
		std::wstring wsTitle = UISTRING_TEXT( "ACCESSORY_ENCHANT_OPTION" );
		if( wsTitle == L"ACCESSORY_ENCHANT_OPTION" )
			wsTitle = L"Available Options";
		ti.SetText( wsTitle.c_str() );
		AddText( &ti, CsPoint( 552, 50 ) );
	}

	{
		cText::sTEXTINFO ti;
		ti.Init( CFont::FS_10, NiColor( 0.45f, 0.75f, 1.0f ) );
		ti.s_eTextAlign = DT_LEFT;
		ti.SetText( L"" );
		for( int i = 0; i < nLimit::MAX_ACCESSORY_OPTION; ++i )
		{
			m_pCurrentOptionText[ i ] = AddText( &ti, CsPoint( 22, 185 + ( i * 24 ) ) );
			m_pCurrentOptionMaxText[ i ] = AddText( &ti, CsPoint( 186, 185 + ( i * 24 ) ) );
			m_pCurrentOptionLockBtn[ i ] = NiNew cSprite;
			if( m_pCurrentOptionLockBtn[ i ] )
			{
				m_pCurrentOptionLockBtn[ i ]->Init( m_pRoot, CsPoint( 372, 176 + ( i * 24 ) ), CsPoint( 32, 32 ), CsRect( 0, 0, 32, 32 ), "Accessory\\Enchant\\UnLock.tga", false );
				m_pCurrentOptionLockBtn[ i ]->SetVisible( false );
				AddChildControl( m_pCurrentOptionLockBtn[ i ] );
			}
		}
	}

	m_pAvailableOptionList = NiNew cListBox;
	if( m_pAvailableOptionList )
	{
		m_pAvailableOptionList->Init( m_pRoot, CsPoint( 407, 86 ), CsPoint( 286, 300 ), NULL, false );
		m_pAvailableOptionList->SetBackOverAndSelectedImgRender( false );
		m_pAvailableOptionList->SetItemHeight( 22 );

		cScrollBar* pScrollBar = NiNew cScrollBar;
		if( pScrollBar )
		{
			pScrollBar->Init( cScrollBar::TYPE_1, NULL, CsPoint::ZERO, CsPoint( 16, 300 ),
				cScrollBar::GetDefaultBtnSize(), CsRect( CsPoint::ZERO, CsPoint( 16, 300 ) ), 13, false );
			pScrollBar->SetEnableRenderFromEnableScroll( true );
			m_pAvailableOptionList->SetScrollBar( pScrollBar );
		}

		AddChildControl( m_pAvailableOptionList );
	}

	{
		cText::sTEXTINFO ti;
		ti.Init( CFont::FS_10, FONT_GREEN );
		ti.s_eTextAlign = DT_CENTER;
		std::wstring wsSameOption = UISTRING_TEXT( "ACCESSORY_ENCHANT_SAME_OPTION" );
		if( wsSameOption == L"ACCESSORY_ENCHANT_SAME_OPTION" )
			wsSameOption = L"Same options can appear";
		ti.SetText( wsSameOption.c_str() );
		m_pSameOptionText = AddText( &ti, CsPoint( 552, 394 ) );
	}
}

void cEnchantOption::_RefreshOptionPanels()
{
	_RefreshStoneCountText();

	const cItemInfo* pAccItem = GetSystem()->GetEnchantItem();

	for( int i = 0; i < nLimit::MAX_ACCESSORY_OPTION; ++i )
	{
		if( m_pCurrentOptionText[ i ] )
			m_pCurrentOptionText[ i ]->SetText( L"" );
		if( m_pCurrentOptionMaxText[ i ] )
			m_pCurrentOptionMaxText[ i ]->SetText( L"" );
		if( m_pCurrentOptionLockBtn[ i ] )
			m_pCurrentOptionLockBtn[ i ]->SetVisible( false );
	}

	if( pAccItem )
	{
		const bool bShowLock = ( GetSystem()->GetEnchantStoneEnchantType() == nItem::AccOption );
		for( int i = 0; i < nLimit::MAX_ACCESSORY_OPTION; ++i )
		{
			if( pAccItem->m_nAccOption[ i ] == 0 )
				continue;

			if( m_pCurrentOptionText[ i ] )
				m_pCurrentOptionText[ i ]->SetText( _GetAccOptionValue( pAccItem->m_nAccOption[ i ], static_cast<float>( pAccItem->m_nAccValues[ i ] ) ).c_str() );

			uint nMaxValue = 0;
			if( m_pCurrentOptionMaxText[ i ] && GetSystem()->GetAccessoryOptionMaxValue( pAccItem->m_nAccOption[ i ], nMaxValue ) )
			{
				std::wstring wsMaxText;
				DmCS::StringFn::Format( wsMaxText, L"[ %s ]", _GetAccOptionValue( pAccItem->m_nAccOption[ i ], static_cast<float>( nMaxValue ) ).c_str() );
				m_pCurrentOptionMaxText[ i ]->SetText( wsMaxText.c_str() );
			}

			if( m_pCurrentOptionLockBtn[ i ] )
			{
				const bool bLocked = GetSystem()->IsOptionLocked( i );
				m_pCurrentOptionLockBtn[ i ]->SetVisible( bShowLock );
				m_pCurrentOptionLockBtn[ i ]->ChangeTexture( bLocked ? "Accessory\\Enchant\\Lock.tga" : "Accessory\\Enchant\\UnLock.tga", CsRect( 0, 0, 32, 32 ) );
			}
		}
	}

	if( m_pAvailableOptionList )
	{
		m_pAvailableOptionList->RemoveAllItem();
		m_pAvailableOptionList->SetScrollBarFirstPos();
	}

	int nAvailableCount = GetSystem()->GetAvailableAccessoryOptionCount();
	if( nAvailableCount <= 0 )
		return;

	float fRate = 100.0f / static_cast<float>( nAvailableCount );
	for( int i = 0; i < nAvailableCount; ++i )
	{
		ushort nOption = 0;
		uint nMinValue = 0;
		uint nMaxValue = 0;
		if( !GetSystem()->GetAvailableAccessoryOption( i, nOption, nMinValue, nMaxValue ) )
			continue;

		cString* pRow = NiNew cString;
		if( !pRow )
			continue;

		{
			cText::sTEXTINFO ti;
			ti.Init( CFont::FS_10, NiColor( 0.45f, 0.75f, 1.0f ) );
			ti.s_eTextAlign = DT_LEFT;
			ti.SetText( GetSystem()->GetOptionName( nOption ).c_str() );
			pRow->AddText( &ti, CsPoint( 0, 0 ) );
		}

		{
			cText::sTEXTINFO ti;
			ti.Init( CFont::FS_10, NiColor( 0.45f, 0.75f, 1.0f ) );
			ti.s_eTextAlign = DT_RIGHT;
			ti.SetText( _GetAvailableOptionValueText( nOption, nMinValue, nMaxValue ).c_str() );
			pRow->AddText( &ti, CsPoint( 218, 0 ) );
		}

		{
			std::wstring wsRate;
			DmCS::StringFn::Format( wsRate, L"%5.2f %%", fRate );

			cText::sTEXTINFO ti;
			ti.Init( CFont::FS_10, NiColor( 0.45f, 0.75f, 1.0f ) );
			ti.s_eTextAlign = DT_RIGHT;
			ti.SetText( wsRate.c_str() );
			pRow->AddText( &ti, CsPoint( 270, 0 ) );
		}

		pRow->SetMaxSize( CsPoint( 270, 22 ) );

		cListBoxItem* pItem = NiNew cListBoxItem;
		if( pItem && m_pAvailableOptionList )
		{
			pItem->SetItem( pRow );
			pItem->SetItemHeight( 22 );
			m_pAvailableOptionList->AddItem( pItem );
		}
		else
		{
			SAFE_NIDELETE( pRow );
			SAFE_NIDELETE( pItem );
		}
	}

	if( m_pAvailableOptionList )
		m_pAvailableOptionList->SetScrollBarFirstPos();
}

std::wstring cEnchantOption::_GetAvailableOptionValueText(ushort nOption, uint nMinValue, uint nMaxValue) const
{
	nOption = static_cast<ushort>( nItem::NormalizeAccessoryOption( nOption ) );
	std::wstring wsText;
	if( nItem::IsAccessoryHundredthPercentOption( nOption ) )
	{
		DmCS::StringFn::Format( wsText, L"%5.2f ~ %5.2f", nMinValue / 100.0f, nMaxValue / 100.0f );
	}
	else
	{
		DmCS::StringFn::Format( wsText, L"%u ~ %u", nMinValue, nMaxValue );
	}

	return wsText;
}

void cEnchantOption::_RefreshStoneCountText()
{
	if( !m_pStoneCountText )
		return;

	const cItemInfo* pStone = GetSystem()->GetEnchantStone();
	if( !pStone )
	{
		m_pStoneCountText->SetText( L"" );
		return;
	}

	std::wstring wsCount;
	DmCS::StringFn::Format( wsCount, L"x%d", GetSystem()->GetRequiredStoneCount() );
	m_pStoneCountText->SetText( wsCount.c_str() );
}

void cEnchantOption::_SetEnchantButton()
{
	SAFE_POINTER_RET( m_pEnchantBtn );
	bool bEnable = m_sEnchantItem.bIsRegist && ( m_sEnchantStone.bIsRegist || GetSystem()->CanAutoRegistEnchantStone() );

	m_pEnchantBtn->SetEnable( bEnable );
}

void cEnchantOption::_EnchantProcessEnd(void* pSender, void* pData)
{
	GetSystem()->SuccessEnchantItem();

	m_sEnchantStone.SetVisible( true );
	m_sEnchantItem.SetVisible( true );
}

//////////////////////////////////////////////////////////////////////////
void cEnchantOption::_OnClickClose(void* pSender, void* pData)
{
	GetSystem()->CloseEnchant();
}

void cEnchantOption::_OnClickEnchant(void* pSender, void* pData)
{
	GetSystem()->EnchantItem();
}

void cEnchantOption::_OnClickComboBox(void* pSender, void* pData)
{
	SAFE_POINTER_RET( pData );
	int nSelectIndex = *static_cast< int* >( pData );

	SAFE_POINTER_RET( m_pOptValCombo );
	int nAccIndex = m_pOptValCombo->GetData( nSelectIndex );
	GetSystem()->SelectOptionValue( nAccIndex );
}

void cEnchantOption::_OnClickOptionLock(void* pSender, void* pData)
{
	for( int i = 0; i < nLimit::MAX_ACCESSORY_OPTION; ++i )
	{
		if( pSender != m_pCurrentOptionLockBtn[ i ] )
			continue;

		_ToggleOptionLock( i );
		return;
	}
}

void cEnchantOption::_ToggleOptionLock(uint nOptionIndex)
{
	if( !GetSystem()->ToggleOptionLock( nOptionIndex ) )
		return;

	_SetEnchantButton();
	_RefreshStoneCountText();
	_RefreshOptionPanels();
}

void cEnchantOption::_OnRClickEnchantStone(void* pSender, void* pData)
{
	if( !GetSystem()->ResetEnchantStone() )
		return;

	m_sEnchantStone.ChangeItem( 0 );
	_RefreshStoneCountText();
	_SetEnchantButton();
	_RefreshOptionPanels();
	if( m_pOptValCombo )
		m_pOptValCombo->SetVisible( false );
}

void cEnchantOption::_OnRClickEnchantItem(void* pSender, void* pData)
{
	if( !GetSystem()->ResetEnchantItem() )
		return;

	m_sEnchantItem.ChangeItem( 0 );
	_SetEnchantButton();
	_RefreshOptionPanels();
	if( m_pOptValCombo )
		m_pOptValCombo->SetVisible( false );

#ifdef LJW_ENCHANT_OPTION_DIGIVICE_190904
	if( m_pItemTypeName )
		m_pItemTypeName->SetText( UISTRING_TEXT( "COMMON_TXT_ACCESSORY" ).c_str() );
#endif
}

void cEnchantOption::_SetTooltipEnchantStone(cListBoxItem const* pOverItem)
{
	SAFE_POINTER_RET( pOverItem );

	const cItemInfo* pEnchant = GetSystem()->GetEnchantStone();
	SAFE_POINTER_RET( pEnchant );
	cItemInfo cEnchantItem = *pEnchant;

	SAFE_POINTER_RET( TOOLTIPMNG_STPTR );
	cTooltip* pTooltip = TOOLTIPMNG_STPTR->GetTooltip();
	SAFE_POINTER_RET( pTooltip );
	pTooltip->SetTooltip( pOverItem->GetWorldPos(), CsPoint( 32, 32 ), TOOLTIP_MAX_SIZE, cTooltip::ITEM, cEnchantItem.m_nType, cBaseWindow::WT_INVENTORY, 0, 0, &cEnchantItem );
}

void cEnchantOption::_SetTooltipEnchantItem(cListBoxItem const* pOverItem)
{
	SAFE_POINTER_RET( pOverItem );

	const cItemInfo* pAccessory = GetSystem()->GetEnchantItem();
	SAFE_POINTER_RET( pAccessory );
	cItemInfo cAccItem = *pAccessory;

	SAFE_POINTER_RET( TOOLTIPMNG_STPTR );
	cTooltip* pTooltip = TOOLTIPMNG_STPTR->GetTooltip();
	SAFE_POINTER_RET( pTooltip );
	pTooltip->SetTooltip( pOverItem->GetWorldPos(), CsPoint( 32, 32 ), TOOLTIP_MAX_SIZE, cTooltip::ITEM, cAccItem.m_nType, cBaseWindow::WT_INVENTORY, 0, 0, &cAccItem );
}

//////////////////////////////////////////////////////////////////////////
void cEnchantOption::sRegistItem::ChangeItem(uint nItemType)
{
	SAFE_POINTER_RET( pItemIcon );
	pItemIcon->ChangeIcon( ICONITEM::ITEM_ICON, nItemType, 1 );
	bIsRegist = ( nItemType == 0 ) ? false : true;
}

void cEnchantOption::sRegistItem::SetVisible(bool bVisible)
{
	SAFE_POINTER_RET( pItemIcon );
	pItemIcon->SetVisible( bVisible );
}
