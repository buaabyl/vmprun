#include <stdio.h>
#include <stdlib.h>
#include "vix.h"

void usage(const char* executive_name)
{
    printf("usage : %s path_to_vmx\n", executive_name);
}

int main(int argc, char* argv[])
{
    VixError err = VIX_OK;
    VixHandle hostHandle = VIX_INVALID_HANDLE;
    VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;

    if (argc != 2)
    {
        usage(argv[0]);
        return -1;
    }

    //Connect to player..
    printf(" * log : connect vixHost...\n");
    jobHandle = VixHost_Connect(VIX_API_VERSION,
                                VIX_SERVICEPROVIDER_VMWARE_PLAYER,
                                NULL, // hostName
                                0, // hostPort
                                NULL, // userName
                                NULL, // password
                                0, // options
                                VIX_INVALID_HANDLE, // propertyListHandle
                                NULL, // callbackProc
                                NULL); // clientData

    //Wait for connect finish.
    err = VixJob_Wait(jobHandle,
                    VIX_PROPERTY_JOB_RESULT_HANDLE,
                    &hostHandle,
                    VIX_PROPERTY_NONE);

    if (VIX_OK != err) {
       // Handle the error...
       goto abort;
    }

    printf(" * log : connect VMWare Player finish.\n");
    Vix_ReleaseHandle(jobHandle);

    //Open virtual machine
    printf(" * log : open virtual machine \"%s\"\n", argv[1]);
    jobHandle = VixVM_Open(hostHandle,
                           argv[1],
                           NULL, // callbackProc
                           NULL); // clientData
    err = VixJob_Wait(jobHandle,
                      VIX_PROPERTY_JOB_RESULT_HANDLE,
                      &vmHandle,
                      VIX_PROPERTY_NONE);
    if (VIX_OK != err) {
       // Handle the error...
       goto abort;
    }

    printf(" * log : open virtual machine finish.\n");
    Vix_ReleaseHandle(jobHandle);

    // Power on the virtual machine before copying file.
    printf(" * log : try to power on machine...\n");
    jobHandle = VixVM_PowerOn(vmHandle,
                              0, // powerOnOptions
                              VIX_INVALID_HANDLE, // propertyListHandle
                              NULL, // callbackProc
                              NULL); // clientData

    err = VixJob_Wait(jobHandle,VIX_PROPERTY_NONE);
    if (VIX_OK != err) {
       // Handle the error...
       goto abort;
    }

    printf(" * log : power on machine finish.\n");
    Vix_ReleaseHandle(jobHandle);

    printf(" * log : wait for machine start up.");
    fflush(stdout);
    while(1)
    {
        // Wait until guest is completely booted.
        jobHandle = VixVM_WaitForToolsInGuest(vmHandle,
                                              1, // timeoutInSeconds
                                              NULL, // callbackProc
                                              NULL); // clientData

        err = VixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
        if (VIX_OK != err) {
           // Handle the error...
            printf(".");
            fflush(stdout);
           //goto abort;
        }
        else
        {
            printf("Finish\n");
            break;
        }

        Vix_ReleaseHandle(jobHandle);
    }

    abort:
    Vix_ReleaseHandle(jobHandle);
    Vix_ReleaseHandle(vmHandle);
    VixHost_Disconnect(hostHandle);

    return 0;
}


