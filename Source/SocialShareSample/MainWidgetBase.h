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
 * SNS sharing type
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
 * @brief Base class for UI for testing SNS sharing functionality
 */
UCLASS(Blueprintable)
class SOCIALSHARESAMPLE_API UMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	//! SNS Share Execution Button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ShareButton;

	//!  Photo to be shared on SNS
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SharePhotoImage;
	
	//! CheckBox if you want to share the photo on SNS
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCheckBox> SharePhotoCheckBox;

	//! CheckBox if you want to share the text on SNS
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCheckBox> ShareTextCheckBox;

protected:
	/**
	* @fn
	* @brief constructor
	* @return
	*/
	virtual void NativeConstruct() override;

public:
	/**
	* @fn
	* @brief  SNS Sharing Execution
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void Share();
	
	/**
	* @fn
	* @brief Temporary export of Texture files to share on SNS 
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void ShareImage();

	/**
	* @fn
	* @brief Share text on social networking sites
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void ShareText();

	/**
	* @fn
	* @brief Temporarily export Texture files and share them with text on SNS
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void ShareTextAndImage();

	/**
	* @fn
	* @brief  Receiving the results of SNS sharing
	* @param (isCompleted) Success or failure
	* @param (ErrorCode)  ErrorCode
	* @param (ErrorDetail) Detailed error message
	* @param (ShareData) Data for SNS sharing
	* @return result
	*/
	UFUNCTION()
	void ReceiveShareResult(bool isCompleted, ESocialShareErrorCode ErrorCode, FString ErrorDetail, USocialShareData* ShareData);

private:
	/**
	* @fn
	* @brief Event when changing the status of the checkbox whether to share the photo or not.
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void OnPhotoShareCheckStateChanged(bool IsShared);

	/**
	* @fn
	* @brief Event on change of status of checkbox whether to share text or not.
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void OnTextShareCheckStateChanged(bool IsShared);

	//!  What to Share
	ESocialShareType SocialShareType;

	/**
	* @fn
	* @brief Update the status of what you share
	* @return
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateSocialShareType();

	//! Delegate handle to receive the results of SNS sharing
	FDelegateHandle ShareResultDelegateHandle;

protected:
	//! Data to Share
	UPROPERTY(BlueprintReadWrite)
	USocialShareData* Data;
};
