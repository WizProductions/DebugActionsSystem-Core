// Copyright Wiz Corporation. All Rights Reserved.


#include "Inputs/DI_IntSlider.h"
#include "Components/SpinBox.h"
#include "SubSystems/DebugSubsystem.h"
#include "WidgetBases/DebugInputSlotBase.h"

void UDI_IntSlider::PostInitProperties() {
	Super::PostInitProperties();

	//In game only
	if (!GetWorld())
		return;

	DebugInputTitle = FText::FromString("Value");
	DebugInputSize = FVector2D(80, 28);
	
	MySpinBox = UDebugSubsystem::Get(GetWorld())->GetNewWidgetInDebugTools<USpinBox>();
	MyWidget = MySpinBox;

	MySpinBox->SetForegroundColor(FLinearColor(0, 0, 0, 1));

	FSpinBoxStyle nSpinBoxStyle = MySpinBox->GetWidgetStyle();
	nSpinBoxStyle.BackgroundBrush.TintColor = FLinearColor(0.75, 0.75, 0.75, 1);

	MySpinBox->SetWidgetStyle(nSpinBoxStyle);
	MySpinBox->SetDelta(1.0f);
	MySpinBox->SetSliderExponent(1.f);
	MySpinBox->SetMinFractionalDigits(0);
	MySpinBox->SetMaxFractionalDigits(0);
}

void UDI_IntSlider::Setup(FString InDebugInputTitle, FIntVector2 BothMinMaxValue, int DefaultValue) {
	
	DebugInputTitle = FText::FromString(InDebugInputTitle);
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

int UDI_IntSlider::GetValue() const {
	return MySpinBox ? MySpinBox->GetValue() : 0;
}
