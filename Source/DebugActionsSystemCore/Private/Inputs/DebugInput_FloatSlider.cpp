// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DebugInput_FloatSlider.h"
#include "Components/SpinBox.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotWidgetBase.h"

UWidget* UDebugInput_FloatSlider::OnConfigureDebugInput_Implementation()
{
	MySlotProperties = FDISlotProperties("Value", FVector2D(80, 28));

	MySpinBox = UDebugSubsystem::Get(this)->GetNewWidgetInDebugPanel<USpinBox>();

	if (MySpinBox == NULL)
		WIZ_RET_LOG(NULL, "My SpinBox is invalid", Error, LogDebugActionsSystem);

	MySpinBox->SetForegroundColor(FLinearColor(0, 0, 0, 1));

	FSpinBoxStyle NewSpinBoxStyle             = MySpinBox->GetWidgetStyle();
	NewSpinBoxStyle.BackgroundBrush.TintColor = FLinearColor(0.75, 0.75, 0.75, 1);

	MySpinBox->SetWidgetStyle(NewSpinBoxStyle);
	MySpinBox->SetDelta(1.0f);
	MySpinBox->SetSliderExponent(1.f);
	MySpinBox->SetMinFractionalDigits(0);
	MySpinBox->SetMaxFractionalDigits(2);

	return MySpinBox;
}

void UDebugInput_FloatSlider::Setup( const FText& InDebugInputTitle, FVector2f BothMinMaxValue, float DefaultValue )
{
	if (MySpinBox == NULL)
		WIZ_RET_LOG(, "My SpinBox is invalid", Error, LogDebugActionsSystem);

	if (InDebugInputTitle.IsEmpty())
		WIZ_LOG("Title is empty", Warning, LogDebugActionsSystem);

	MySpinBox->SetValue(MySpinBox->GetValue() > BothMinMaxValue.Y
		                    ? BothMinMaxValue.Y
		                    : MySpinBox->GetValue() < BothMinMaxValue.X
			                      ? BothMinMaxValue.X
			                      : MySpinBox->GetValue());

	if (DefaultValue != 0) {
		//Temp adjust min value as default value, otherwise the slider may not accept it
		MySpinBox->SetMinValue(DefaultValue);
		MySpinBox->SetValue(DefaultValue);
	}

	MySpinBox->SetMinSliderValue(BothMinMaxValue.X);
	MySpinBox->SetMinValue(BothMinMaxValue.X);
	MySpinBox->SetMaxSliderValue(BothMinMaxValue.Y);
	MySpinBox->SetMaxValue(BothMinMaxValue.Y);

	MySlotProperties.DisplayName = InDebugInputTitle;
	MyDebugInputSlotWidget->SetSlotProperties(MySlotProperties);
}

float UDebugInput_FloatSlider::GetValue() const
{
	if (MySpinBox == NULL)
		WIZ_RET_LOG(0.0f, "My SpinBox is invalid", Error, LogDebugActionsSystem);

	return MySpinBox->GetValue();
}
