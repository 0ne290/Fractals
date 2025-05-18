#include "JsonSerializer.h"
#include "Converter.h"
#include <format>
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>

namespace Fractals::Infrastructure
{
    JsonSerializer::JsonSerializer(const Fractals::Infrastructure::SharedConverter& converter)
        : _converter(converter) { }

    SharedJsonSerializer JsonSerializer::Create(const Fractals::Infrastructure::SharedConverter& converter)
    {
        return MAKE_SHARED_JSON_SERIALIZER(converter);
    }

    SharedString JsonSerializer::ToJson(const SharedVector<const char*> strs)
    {
        const int strsLastIndex = strs->size() - 1;
        
        SharedString ret = MAKE_SHARED_STRING("[");

        if (strsLastIndex >= 0)
		{
			for (auto i = 0; i < strsLastIndex; i++)
				ret->append(std::format(R"("{}",)", (*strs)[i]));

			ret->append(std::format(R"("{}")", (*strs)[strsLastIndex]));
		}

        ret->append("]");

        return ret;
    }

    SharedString JsonSerializer::WrapInQuotes(const SharedString str)
    {
        return MAKE_SHARED_STRING(std::format(R"("{}")", *str));
    }

	SharedString JsonSerializer::ToJson(const Shared<VkPhysicalDeviceProperties> properties)
	{
		return MAKE_SHARED_STRING(std::format(
			R"({{"apiVersion":{},"driverVersion":{},"vendorId":{},"deviceId":{},"deviceType":"{}","deviceName":"{}"}})",
			properties->apiVersion,
			properties->driverVersion,
			properties->vendorID,
			properties->deviceID,
			*_converter->ToString(properties->deviceType),
			properties->deviceName
		));
	}

	SharedString JsonSerializer::ToJson(const Shared<VkPhysicalDeviceFeatures> features)
	{
		return MAKE_SHARED_STRING(std::format(
			R"({{"robustBufferAccess":{},"fullDrawIndexUint32":{},"imageCubeArray":{},"independentBlend":{},"geometryShader":{},"tessellationShader":{},"sampleRateShading":{},"dualSrcBlend":{},"logicOp":{},"multiDrawIndirect":{},"drawIndirectFirstInstance":{},"depthClamp":{},"depthBiasClamp":{},"fillModeNonSolid":{},"depthBounds":{},"wideLines":{},"largePoints":{},"alphaToOne":{},"multiViewport":{},"samplerAnisotropy":{},"textureCompressionETC2":{},"textureCompressionASTC_LDR":{},"textureCompressionBC":{},"occlusionQueryPrecise":{},"pipelineStatisticsQuery":{},"vertexPipelineStoresAndAtomics":{},"fragmentStoresAndAtomics":{},"shaderTessellationAndGeometryPointSize":{},"shaderImageGatherExtended":{},"shaderStorageImageExtendedFormats":{},"shaderStorageImageMultisample":{},"shaderStorageImageReadWithoutFormat":{},"shaderStorageImageWriteWithoutFormat":{},"shaderUniformBufferArrayDynamicIndexing":{},"shaderSampledImageArrayDynamicIndexing":{},"shaderStorageBufferArrayDynamicIndexing":{},"shaderStorageImageArrayDynamicIndexing":{},"shaderClipDistance":{},"shaderCullDistance":{},"shaderFloat64":{},"shaderInt64":{},"shaderInt16":{},"shaderResourceResidency":{},"shaderResourceMinLod":{},"sparseBinding":{},"sparseResidencyBuffer":{},"sparseResidencyImage2D":{},"sparseResidencyImage3D":{},"sparseResidency2Samples":{},"sparseResidency4Samples":{},"sparseResidency8Samples":{},"sparseResidency16Samples":{},"sparseResidencyAliased":{},"variableMultisampleRate":{},"inheritedQueries":{}}})",
			*_converter->ToString(features->robustBufferAccess),
			*_converter->ToString(features->fullDrawIndexUint32),
			*_converter->ToString(features->imageCubeArray),
			*_converter->ToString(features->independentBlend),
			*_converter->ToString(features->geometryShader),
			*_converter->ToString(features->tessellationShader),
			*_converter->ToString(features->sampleRateShading),
			*_converter->ToString(features->dualSrcBlend),
			*_converter->ToString(features->logicOp),
			*_converter->ToString(features->multiDrawIndirect),
			*_converter->ToString(features->drawIndirectFirstInstance),
			*_converter->ToString(features->depthClamp),
			*_converter->ToString(features->depthBiasClamp),
			*_converter->ToString(features->fillModeNonSolid),
			*_converter->ToString(features->depthBounds),
			*_converter->ToString(features->wideLines),
			*_converter->ToString(features->largePoints),
			*_converter->ToString(features->alphaToOne),
			*_converter->ToString(features->multiViewport),
			*_converter->ToString(features->samplerAnisotropy),
			*_converter->ToString(features->textureCompressionETC2),
			*_converter->ToString(features->textureCompressionASTC_LDR),
			*_converter->ToString(features->textureCompressionBC),
			*_converter->ToString(features->occlusionQueryPrecise),
			*_converter->ToString(features->pipelineStatisticsQuery),
			*_converter->ToString(features->vertexPipelineStoresAndAtomics),
			*_converter->ToString(features->fragmentStoresAndAtomics),
			*_converter->ToString(features->shaderTessellationAndGeometryPointSize),
			*_converter->ToString(features->shaderImageGatherExtended),
			*_converter->ToString(features->shaderStorageImageExtendedFormats),
			*_converter->ToString(features->shaderStorageImageMultisample),
			*_converter->ToString(features->shaderStorageImageReadWithoutFormat),
			*_converter->ToString(features->shaderStorageImageWriteWithoutFormat),
			*_converter->ToString(features->shaderUniformBufferArrayDynamicIndexing),
			*_converter->ToString(features->shaderSampledImageArrayDynamicIndexing),
			*_converter->ToString(features->shaderStorageBufferArrayDynamicIndexing),
			*_converter->ToString(features->shaderStorageImageArrayDynamicIndexing),
			*_converter->ToString(features->shaderClipDistance),
			*_converter->ToString(features->shaderCullDistance),
			*_converter->ToString(features->shaderFloat64),
			*_converter->ToString(features->shaderInt64),
			*_converter->ToString(features->shaderInt16),
			*_converter->ToString(features->shaderResourceResidency),
			*_converter->ToString(features->shaderResourceMinLod),
			*_converter->ToString(features->sparseBinding),
			*_converter->ToString(features->sparseResidencyBuffer),
			*_converter->ToString(features->sparseResidencyImage2D),
			*_converter->ToString(features->sparseResidencyImage3D),
			*_converter->ToString(features->sparseResidency2Samples),
			*_converter->ToString(features->sparseResidency4Samples),
			*_converter->ToString(features->sparseResidency8Samples),
			*_converter->ToString(features->sparseResidency16Samples),
			*_converter->ToString(features->sparseResidencyAliased),
			*_converter->ToString(features->variableMultisampleRate),
			*_converter->ToString(features->inheritedQueries)
		));
	}
}