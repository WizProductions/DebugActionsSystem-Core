// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_FloatSlider.h"
#include "Components/SpinBox.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotBase.h"

void UDI_FloatSlider::PostInitProperties() {
	Super::PostInitProperties();

	//In game only
	if (!GetWorld())
		return;

	DebugInputTitle = FText::FromString("Value");
	DebugInputSize = FVector2D(80, 28);
	
	MySpinBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugPanel<USpinBox>();
	MyWidget = MySpinBox;

	MySpinBox->SetForegroundColor(FLinearColor(0, 0, 0, 1));

	FSpinBoxStyle NewSpinBoxStyle = MySpinBox->GetWidgetStyle();
	NewSpinBoxStyle.BackgroundBrush.TintColor = FLinearColor(0.75, 0.75, 0.75, 1);

	MySpinBox->SetWidgetStyle(NewSpinBoxStyle);
	MySpinBox->SetDelta(1.0f);
	MySpinBox->SetSliderExponent(1.f);
	MySpinBox->SetMinFractionalDigits(0);
	MySpinBox->SetMaxFractionalDigits(2);
}

void UDI_FloatSlider::Setup(FText InDebugInputTitle, FVector2f BothMinMaxValue, float DefaultValue) {
	
	DebugInputTitle = InDebugInputTitle;
	MySpinBox->SetValue(MySpinBox->GetValue() > BothMinMaxValue.Y ? BothMinMaxValue.Y : MySpinBox->GetValue() < BothMinMaxValue.X ? BothMinMaxValue.X : MySpinBox->GetValue());
	
	if (DefaultValue != 0) {
		//Temp adjust min value as default value, otherwise the slider may not accept it
		MySpinBox->SetMinValue(DefaultValue);
		MySpinBox->SetValue(DefaultValue);
	}
	
	MySpinBox->SetMinSliderValue(BothMinMaxValue.X);
	MySpinBox->SetMinValue(BothMinMaxValue.X);
	MySpinBox->SetMaxSliderValue(BothMinMaxValue.Y);
	MySpinBox->SetMaxValue(BothMinMaxValue.Y);
	
	MyDebugInputSlot->SetTitle(DebugInputTitle);
}

float UDI_FloatSlider::GetValue() const {
	return MySpinBox ? MySpinBox->GetValue() : 0.0f;
}
