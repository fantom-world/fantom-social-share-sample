// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "SocialShareBPLibrary.h"
#include "MainWidgetBase.generated.h"

/**
 * @enum ESocialShareType
 * SNS共有のタイプ
 */
UENUM(BlueprintType)
enum class ESocialShareType : uint8
{
	None,
	Text,
	Image,
	TextAndImage
};

/**
 * @class UMainWidgetBase
 * @brief SNS共有機能テスト用のUIのベースクラス
 */
UCLASS(Blueprintable)
class SOCIALSHARESAMPLE_API UMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	//! SNSシェア用Button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ShareButton;

	//! SNSシェアする写真
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SharePhotoImage;
	
	//! 写真をSNSシェアするかどうかのCheckBox
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCheckBox> SharePhotoCheckBox;

	//! テキストをSNSシェアするかどうかのCheckBox
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCheckBox> ShareTextCheckBox;

protected:
	/**
	* @fn
	* @brief コンストラクタ
	* @return
	*/
	virtual void NativeConstruct() override;

public:
	/**
	* @fn
	* @brief SNS共有
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void Share();
	
	/**
	* @fn
	* @brief Textureファイルを一時的に書き出してSNSで共有
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void ShareImage();

	/**
	* @fn
	* @brief テキストをSNSで共有
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void ShareText();

	/**
	* @fn
	* @brief Textureファイルを一時的に書き出してテキストと共にSNSで共有
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void ShareTextAndImage();

	UFUNCTION()
	void ReceiveShareResult(bool isCompleted, ESocialShareErrorCode ErrorCode, FString ErrorDetail, USocialShareData* ShareData);

private:
	/**
	* @fn
	* @brief 写真をシェアするかどうかのチェックボックスの状態変更時のイベント
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void OnPhotoShareCheckStateChanged(bool IsShared);

	/**
	* @fn
	* @brief テキストをシェアするかどうかのチェックボックスの状態変更時のイベント
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void OnTextShareCheckStateChanged(bool IsShared);

	//!  何をシェアするか
	ESocialShareType SocialShareType;

	/**
	* @fn
	* @brief 何をシェアするかの状態を更新する
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateSocialShareType();

	FDelegateHandle ShareResultDelegateHandle;

protected:
	//! Shareするデータ
	UPROPERTY(BlueprintReadWrite)
	USocialShareData* Data;
};
