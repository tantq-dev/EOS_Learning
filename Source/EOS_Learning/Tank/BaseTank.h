// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseTank.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;


UCLASS()
class EOS_LEARNING_API ABaseTank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseTank();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* Arm;
	 
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;


};
