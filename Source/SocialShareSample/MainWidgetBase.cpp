// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetBase.h"
#include "SocialShareData.h"

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
    if (USocialShareBPLibrary::GetIsProcessing())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Other SNS sharing is in progress.")));
        return;
    }

    //SNS共有
    UTexture* ImageSource = Cast<UTexture>(SharePhotoImage->GetBrush().GetResourceObject());
    if (ImageSource)
    {
        Data = NewObject<USocialShareData>();
        Data->Initialize(ImageSource);
        Data->Delegate.AddDynamic(this, &UMainWidgetBase::ReceiveShareResult);

        if (!USocialShareBPLibrary::Share(Data))
        {
            UE_LOG(LogTemp, Warning, TEXT("falied: ShareImage"));
        }
    }
    
}

void UMainWidgetBase::ShareText()
{
    if (USocialShareBPLibrary::GetIsProcessing())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Other SNS sharing is in progress.")));
        return;
    }

    Data = NewObject<USocialShareData>();
    Data->Initialize(
        FString::Printf(TEXT("テキストを共有")), 
        FString::Printf(TEXT("Favorite Space")), 
        FString::Printf(TEXT("好きで、つながろう\nhttps://play.google.com/store/apps/details?id=com.sony.snspace\n#FavoriteSpace"))
    );
    Data->Delegate.AddDynamic(this, &UMainWidgetBase::ReceiveShareResult);
    if (!USocialShareBPLibrary::Share(Data))
    {
        UE_LOG(LogTemp,Warning, TEXT("falied: UMainWidgetBase::ShareTex"));
    }
}

void UMainWidgetBase::ShareTextAndImage()
{
    
    if (USocialShareBPLibrary::GetIsProcessing())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Other SNS sharing is in progress.")));
        return;
    }
 
    UTexture* ImageSource = Cast<UTexture>(SharePhotoImage->GetBrush().GetResourceObject());
    if (ImageSource)
    {
        Data = NewObject<USocialShareData>();
        Data->Initialize(
            FString::Printf(TEXT("画像を共有")),
            FString::Printf(TEXT("Favorite Space")),
            FString::Printf(TEXT("好きで、つながろう\nhttps://play.google.com/store/apps/details?id=com.sony.snspace\n#FavoriteSpace")),
            ImageSource
        );
        Data->Delegate.AddDynamic(this, &UMainWidgetBase::ReceiveShareResult);

        if (!USocialShareBPLibrary::Share(Data))
        {
            UE_LOG(LogTemp, Warning, TEXT("falied: ShareImageAndText"));         
        }
    }
  
}


void UMainWidgetBase::ReceiveShareResult(bool isCompleted, ESocialShareErrorCode ErrorCode, FString ErrorDetail, USocialShareData* ShareData)
{
    if (ShareData && ShareData == Data)
    {
        if (isCompleted)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Sharing is completed: %s"), *ShareData->ShareTarget));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Sharing is failed!: %i %s"), ErrorCode, *ErrorDetail));
        }

        if (ShareResultDelegateHandle.IsValid())
        {
           ShareData->Delegate.RemoveDynamic(this, &UMainWidgetBase::ReceiveShareResult);
           ShareResultDelegateHandle.Reset();
        }
        Data = nullptr;
    }
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

