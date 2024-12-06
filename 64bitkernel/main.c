//
// Created by vitalyv on 12/6/24.
//
#include <efi.h>
#include <efilib.h>

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    // Initialize the UEFI library
    InitializeLib(ImageHandle, SystemTable);

    // Clear the screen
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    // Print strings to the console
    Print(L"Hello World!\n");
    Print(L"Goodbye World!\n");

    // Wait for a keystroke before exiting
    Print(L"Press any key to exit...\n");
    WaitForSingleEvent(SystemTable->ConIn->WaitForKey, 0);

    return EFI_SUCCESS;
}
