// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetBase.h"
#include "SocialShareBPLibrary.h"

void UMainWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    //イベントをバインド
    ShareButton->OnClicked.AddDynamic(this, &UMainWidgetBase::Share);
    SharePhotoCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainWidgetBase::OnPhotoShareCheckStateChanged);
    ShareTextCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainWidgetBase::OnTextShareCheckStateChanged);
    
    UpdateSocialShareType();
}

void UMainWidgetBase::Share()
{
    switch (SocialShareType)
    {
    case ESocialShareType::Text:
        ShareText();
        break;
    case ESocialShareType::Image:
        ShareImage();
        break;
    case ESocialShareType::TextAndImage:
        ShareTextAndImage();
        break;
    default:
        break;
    }
}

void UMainWidgetBase::ShareImage()
{
    //SNS共有時に選択したアプリ名を通知するイベントをBindする
    OnReceiveShareSelectedAppDelegateHandle = USocialShareBPLibrary::OnReceiveShareSelectedAppDelegate.AddUObject(this, &UMainWidgetBase::PrintShareAppName);

    //SNS共有
    UTexture* ImageSource = Cast<UTexture>(SharePhotoImage->GetBrush().GetResourceObject());
    USocialShareBPLibrary::ShareImageFromTexture(ImageSource);
}

void UMainWidgetBase::ShareText()
{
    USocialShareBPLibrary::ShareText(
        TEXT("画像を共有"),
        TEXT("Favorite Space"),
        TEXT("好きで、つながろう\nhttps://play.google.com/store/apps/details?id=com.sony.snspace\n#FavoriteSpace")
    );
}

void UMainWidgetBase::ShareTextAndImage()
{
    //SNS共有時に選択したアプリ名を通知するイベントをBindする
    OnReceiveShareSelectedAppDelegateHandle = USocialShareBPLibrary::OnReceiveShareSelectedAppDelegate.AddUObject(this, &UMainWidgetBase::PrintShareAppName);

    UTexture* ImageSource = Cast<UTexture>(SharePhotoImage->GetBrush().GetResourceObject());
    USocialShareBPLibrary::ShareImageAndTextFromTexture(
        TEXT("画像を共有"),
        TEXT("Favorite Space"),
        TEXT("好きで、つながろう\nhttps://play.google.com/store/apps/details?id=com.sony.snspace\n#FavoriteSpace"),
        ImageSource
    );
}

void UMainWidgetBase::PrintShareAppName(FString AppName)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *AppName));
    USocialShareBPLibrary::OnReceiveShareSelectedAppDelegate.Remove(OnReceiveShareSelectedAppDelegateHandle);
    OnReceiveShareSelectedAppDelegateHandle.Reset();
}

void UMainWidgetBase::OnPhotoShareCheckStateChanged(bool IsShared)
{
    UpdateSocialShareType();

    SharePhotoImage->SetIsEnabled(IsShared);
}

void UMainWidgetBase::OnTextShareCheckStateChanged(bool IsShared)
{
    UpdateSocialShareType();
}

void UMainWidgetBase::UpdateSocialShareType()
{
    if (ShareTextCheckBox->IsChecked() && SharePhotoCheckBox->IsChecked())
    {
        SocialShareType = ESocialShareType::TextAndImage;
    }
    else if (!ShareTextCheckBox->IsChecked() && SharePhotoCheckBox->IsChecked())
    {
        SocialShareType = ESocialShareType::Image;
    }
    else if (ShareTextCheckBox->IsChecked() && !SharePhotoCheckBox->IsChecked())
    {
        SocialShareType = ESocialShareType::Text;
    }
    else
    {
        SocialShareType = ESocialShareType::None;
    }
    
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *StaticEnum<ESocialShareType>()->GetValueAsString(SocialShareType)));
}

