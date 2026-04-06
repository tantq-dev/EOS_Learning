// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank/BaseTank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABaseTank::ABaseTank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComp);
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMEsh"));
	BaseMesh->SetupAttachment(CapsuleComp);

}

// Called when the game starts or when spawned
void ABaseTank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

