/* Copyright (c) Microsoft Corporation.
 * Licensed under the MIT License. */

/**
 * @file azure_iot.h
 *
 * @brief Azure IoT FreeRTOS middleware common APIs and structs
 *
 * @note You MUST NOT use any symbols (macros, functions, structures, enums, etc.)
 * prefixed with an underscore ('_') directly in your application code. These symbols
 * are part of Azure SDK's internal implementation; we do not document these symbols
 * and they are subject to change in future versions of the SDK which would break your code.
 *
 */

#ifndef AZURE_IOT_H
#define AZURE_IOT_H

/* AZURE_IOT_NO_CUSTOM_CONFIG allows building the azure iot library
 * without a custom config. If a custom config is provided, the
 * AZURE_IOT_NO_CUSTOM_CONFIG macro should not be defined. */
#ifndef AZURE_IOT_NO_CUSTOM_CONFIG
    /* Include custom config file before other headers. */
    #include "azure_iot_config.h"
#endif

#include "azure_iot_result.h"

/* Include config defaults header to get default values of configs not
 * defined in azure_iot_mqtt_config.h file. */
#include "azure_iot_config_defaults.h"

#include "FreeRTOS.h"

#include "azure/az_iot.h"

#include <azure/core/_az_cfg_prefix.h>

/**
 * @brief Milliseconds per FreeRTOS tick.
 */
#define azureiotMILLISECONDS_PER_TICK    ( 1000 / configTICK_RATE_HZ )

/**
 * @brief The bag of properties associated with a message.
 *
 */
typedef struct AzureIoTMessageProperties
{
    struct
    {
        az_iot_message_properties xProperties;
    } _internal; /**< @brief Internal to the SDK */
} AzureIoTMessageProperties_t;

/**
 * @brief The platform get time function to be used by the SDK for MQTT connections.
 *
 * @return The time (in seconds) since Unix epoch.
 */
typedef uint64_t ( * AzureIoTGetCurrentTimeFunc_t )( void );

/**
 * @brief The HMAC256 function used by the SDK to generate SAS keys.
 *
 * @param[in] pucKey The key to use for the HMAC operation.
 * @param[in] ulKeyLength The length of \p pucKey.
 * @param[in] pucData The data on which the operation will take place. In this context, the data
 * will be a certain concatenation of the iot hub name, sas key, and expiration time.
 * @param[in] ulDataLength The length of \p pucData.
 * @param[in,out] pucOutput The buffer into which the processed data will be placed.
 * @param[in] ulOutputLength The size of \p pucOutput.
 * @param[out] pulBytesCopied The number of bytes copied into \p pucOutput.
 */
typedef uint32_t ( * AzureIoTGetHMACFunc_t )( const uint8_t * pucKey,
                                              uint32_t ulKeyLength,
                                              const uint8_t * pucData,
                                              uint32_t ulDataLength,
                                              uint8_t * pucOutput,
                                              uint32_t ulOutputLength,
                                              uint32_t * pulBytesCopied );

/**
 * @brief Initialize Azure IoT middleware.
 *
 * @note This should be called once per process.
 *
 */
AzureIoTResult_t AzureIoT_Init();

/**
 * @brief Deinitialize Azure IoT middleware.
 *
 * @note This should be called once per process.
 *
 */
void AzureIoT_Deinit();

/**
 * @brief Initialize the message properties.
 *
 * @note The properties init API will not encode properties. In order to support
 *       the following characters, they must be percent-encoded (RFC3986) as follows:
 *         - `/` : `%2F`
 *         - `%` : `%25`
 *         - `#` : `%23`
 *         - `&` : `%26`
 *       Only these characters would have to be encoded. If you would like to avoid the need to
 *       encode the names/values, avoid using these characters in names and values.
 *
 * @param[out] pxMessageProperties The #AzureIoTMessageProperties_t* to use for the operation.
 * @param[out] pucBuffer The pointer to the buffer.
 * @param[in] ulAlreadyWrittenLength The length of the properties already written (if applicable).
 * @param[in] ulBufferLength The length of \p pucBuffer.
 * @return An #AzureIoTResult_t with the result of the operation.
 */
AzureIoTResult_t AzureIoT_MessagePropertiesInit( AzureIoTMessageProperties_t * pxMessageProperties,
                                                 uint8_t * pucBuffer,
                                                 uint32_t ulAlreadyWrittenLength,
                                                 uint32_t ulBufferLength );

/**
 * @brief Append a property name and value to a message.
 *
 * @note The properties init API will not encode properties. In order to support
 *       the following characters, they must be percent-encoded (RFC3986) as follows:
 *         - `/` : `%2F`
 *         - `%` : `%25`
 *         - `#` : `%23`
 *         - `&` : `%26`
 *       Only these characters would have to be encoded. If you would like to avoid the need to
 *       encode the names/values, avoid using these characters in names and values.
 *
 * @param[in] pxMessageProperties The #AzureIoTMessageProperties_t* to use for the operation.
 * @param[in] pucName The name of the property to append.
 * @param[in] ulNameLength The length of \p pucName.
 * @param[in] pucValue The value of the property to append.
 * @param[in] ulValueLength The length of \p pucValue.
 * @return An #AzureIoTResult_t with the result of the operation.
 */
AzureIoTResult_t AzureIoT_MessagePropertiesAppend( AzureIoTMessageProperties_t * pxMessageProperties,
                                                   const uint8_t * pucName,
                                                   uint32_t ulNameLength,
                                                   const uint8_t * pucValue,
                                                   uint32_t ulValueLength );

/**
 * @brief Find a property in the message property bag.
 *
 * @param[in] pxMessageProperties The #AzureIoTMessageProperties_t* to use for the operation.
 * @param[in] pucName The name of the property to find.
 * @param[in] ulNameLength Length of the property name.
 * @param[out] ppucOutValue The output pointer to the property value.
 * @param[out] pulOutValueLength The length of \p ppucOutValue.
 * @return An #AzureIoTResult_t with the result of the operation.
 */
AzureIoTResult_t AzureIoT_MessagePropertiesFind( AzureIoTMessageProperties_t * pxMessageProperties,
                                                 const uint8_t * pucName,
                                                 uint32_t ulNameLength,
                                                 const uint8_t ** ppucOutValue,
                                                 uint32_t * pulOutValueLength );

#include <azure/core/_az_cfg_suffix.h>

#endif /* AZURE_IOT_H */
