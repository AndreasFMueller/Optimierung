/*
 * clutils.c
 *
 * (c) 2011 Prof Dr Andreas Mueller, Hochschule Rapperswil 
 * $Id$
 */

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <alloca.h>
#include <string.h>
#include <errno.h>

#include <globals.h>
#include <clutils.h>

/*
 * create a program given the CL file name
 */
cl_program	cluCreateProgramWithFilename(cl_context context,
		const char *filename, cl_int *err) {
	struct stat	sb;
	if (stat(filename, &sb) < 0) {
		*err = -1;
		return NULL;
	}
	char	*buffer = (char *)alloca(sb.st_size + 1);
	if (NULL == buffer) {
		*err = -1;
		return NULL;
	}
	buffer[sb.st_size] = '\0';
	int	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		*err = -1;
		return NULL;
	}
	if (sb.st_size != read(fd, buffer, sb.st_size)) {
		*err = -1;
		close(fd);
		return NULL;
	}
	close(fd);
	return clCreateProgramWithSource(context, 1, (const char **)&buffer, NULL, err);
}

/*
 * utility function to show the platorm information
 */
void	clu_showplatforminfo(FILE *output, cl_platform_id platform) {
	char	buffer[1024];
	size_t	size;
	cl_int	err = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE,
			sizeof(buffer), buffer, &size);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot get platform profile");
	} else {
		fprintf(output, "platform profile:                %s\n", buffer);
	}

	err = clGetPlatformInfo(platform, CL_PLATFORM_VERSION,
		sizeof(buffer), buffer, &size);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot get platform version");
	} else {
		fprintf(output, "platform version:                %s\n", buffer);
	}

	err = clGetPlatformInfo(platform, CL_PLATFORM_NAME,
		sizeof(buffer), buffer, &size);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot get platform name");
	} else {
		fprintf(output, "platform name:                   %s\n", buffer);
	}

	err = clGetPlatformInfo(platform, CL_PLATFORM_VENDOR,
		sizeof(buffer), buffer, &size);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot get platform vendor");
	} else {
		fprintf(output, "platform vendor:                 %s\n", buffer);
	}

	err = clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS,
		sizeof(buffer), buffer, &size);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot get platform extensions");
	} else {
		fprintf(output, "platform extensions:             %s\n", buffer);
	}
}

/*
 * utility function to show device information
 */
void	clu_showdeviceinfo(FILE *output, cl_device_id device) {
	// device type
	cl_device_type	device_type;
	clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_device_type), 
		&device_type, NULL);
	fprintf(output, "device type:                         ");
	if (device_type & CL_DEVICE_TYPE_CPU) {
		fprintf(output, " CPU");
	}
	if (device_type & CL_DEVICE_TYPE_GPU) {
		fprintf(output, " GPU");
	}
	if (device_type & CL_DEVICE_TYPE_ACCELERATOR) {
		fprintf(output, " ACCELERATOR");
	}
	if (device_type & CL_DEVICE_TYPE_DEFAULT) {
		fprintf(output, " DEFAULT");
	}
	fprintf(output, "\n");

	// device vendor id
	cl_uint	vendor_id;
	clGetDeviceInfo(device, CL_DEVICE_VENDOR_ID, sizeof(cl_uint),
		&vendor_id, NULL);
	fprintf(output, "device vendor id:                     %u\n",
		vendor_id);

	// compute units
	cl_uint	compute_units;
	clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS,
		sizeof(compute_units), &compute_units, NULL);
	fprintf(output, "device max compute units:             %d\n",
		compute_units);

	// work item dimensions
	cl_uint	work_item_dimensions;
	clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
		sizeof(work_item_dimensions), &work_item_dimensions, NULL);
	fprintf(output, "device max work item dimensions:      %d\n",
		work_item_dimensions);

	// work item sizes
	size_t	max_work_item_sizes[3];
	clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES,
		sizeof(max_work_item_sizes), max_work_item_sizes, NULL);
	fprintf(output, "device max work item sizes:           (%ld,%ld,%ld)\n",
		max_work_item_sizes[0],
		max_work_item_sizes[1],
		max_work_item_sizes[2]);

	// max work group size
	size_t	max_work_group_size;
	clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE,
		sizeof(max_work_group_size), &max_work_group_size, NULL);
	fprintf(output, "device max work group size:           %ld\n",
		max_work_group_size);

	// prefered vector widths
	cl_uint	preferred;
	clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
		sizeof(cl_uint), &preferred, NULL);
	fprintf(output, "device preferred vector width char:   %d\n",
		preferred);
	clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
		sizeof(cl_uint), &preferred, NULL);
	fprintf(output, "device preferred vector width short:  %d\n",
		preferred);
	clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
		sizeof(cl_uint), &preferred, NULL);
	fprintf(output, "device preferred vector width int:    %d\n",
		preferred);
	clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
		sizeof(cl_uint), &preferred, NULL);
	fprintf(output, "device preferred vector width long:   %d\n",
		preferred);
	clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
		sizeof(cl_uint), &preferred, NULL);
	fprintf(output, "device preferred vector width float:  %d\n",
		preferred);
	clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
		sizeof(cl_uint), &preferred, NULL);
	fprintf(output, "device preferred vector width double: %d\n",
		preferred);

	// glock frequency
	cl_uint	clock_frequency;
	clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY,
		sizeof(clock_frequency), &clock_frequency, NULL);
	fprintf(output, "device max clock frequency:           %u\n",
		clock_frequency);

	// device address bits
	cl_uint	device_address_bits;
	clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, sizeof(cl_uint),
		&device_address_bits, NULL);
	fprintf(output, "device address bits:                  %u\n",
		device_address_bits);

	// maximum memory allocation size
	cl_ulong	mem_alloc_size;
	clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE,
		sizeof(mem_alloc_size), &mem_alloc_size, NULL);
	fprintf(output, "device max mem alloc size:            %lu\n",
		mem_alloc_size);

	// image support
	cl_bool	image_support;
	clGetDeviceInfo(device, CL_DEVICE_IMAGE_SUPPORT,
		sizeof(image_support), &image_support, NULL);
	fprintf(output, "device image support:                 %s\n",
		(image_support) ? "yes" : "no");

	// maximum read image arguments
	cl_uint	max_read_image_args;
	clGetDeviceInfo(device, CL_DEVICE_MAX_READ_IMAGE_ARGS,
		sizeof(max_read_image_args), &max_read_image_args, NULL);
	fprintf(output, "device max read image args:           %u\n",
		max_read_image_args);

	// maximum write image arguments
	cl_uint	max_write_image_args;
	clGetDeviceInfo(device, CL_DEVICE_MAX_WRITE_IMAGE_ARGS,
		sizeof(max_write_image_args), &max_write_image_args, NULL);
	fprintf(output, "device max write image args:          %u\n",
		max_write_image_args);

	// max width
	size_t	image2d_max_width;
	clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_WIDTH,
		sizeof(image2d_max_width), &image2d_max_width, NULL);
	fprintf(output, "device image2d max width:             %ld\n",
		image2d_max_width);

	// max height
	size_t	image2d_max_height;
	clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_HEIGHT,
		sizeof(image2d_max_height), &image2d_max_height, NULL);
	fprintf(output, "device image2d max height:            %ld\n",
		image2d_max_height);

	// max width 3d
	size_t	image3d_max_width;
	clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_WIDTH,
		sizeof(image3d_max_width), &image3d_max_width, NULL);
	fprintf(output, "device image3d max width:             %ld\n",
		image3d_max_width);

	// max height 3d
	size_t	image3d_max_height;
	clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_HEIGHT,
		sizeof(image3d_max_height), &image3d_max_height, NULL);
	fprintf(output, "device image3d max height:            %ld\n",
		image3d_max_height);

	// max depth 3d
	size_t	image3d_max_depth;
	clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_DEPTH,
		sizeof(image3d_max_depth), &image3d_max_depth, NULL);
	fprintf(output, "device image3d max depth:             %ld\n",
		image3d_max_depth);

	// max samplers
	cl_uint	max_samplers;
	clGetDeviceInfo(device, CL_DEVICE_MAX_SAMPLERS,
		sizeof(max_samplers), &max_samplers, NULL);
	fprintf(output, "device max samplers:                  %u\n",
		max_samplers);

	// max parameter size
	size_t	parameter_size;
	clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE,
		sizeof(parameter_size), &parameter_size, NULL);
	fprintf(stdout, "device max parameter size:            %ld\n",
		parameter_size);

	// device mem base addr align
	cl_uint	base_align;
	clGetDeviceInfo(device, CL_DEVICE_MEM_BASE_ADDR_ALIGN,
		sizeof(base_align), &base_align, NULL);
	fprintf(output, "device mem base addr align:           %u\n",
		base_align);

	// min data type align
	cl_uint	min_datatype_align;
	clGetDeviceInfo(device, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE,
		sizeof(min_datatype_align), &min_datatype_align, NULL);
	fprintf(output, "device min data type align size:      %u\n",
		min_datatype_align);

	// fp config
	cl_device_fp_config	fp_config;
	clGetDeviceInfo(device, CL_DEVICE_SINGLE_FP_CONFIG,
		sizeof(fp_config), &fp_config, NULL);
	fprintf(output, "device fp configuration:             ");
	if (fp_config & CL_FP_DENORM) {
		fprintf(output, " DENORM");
	}
	if (fp_config & CL_FP_INF_NAN) {
		fprintf(output, " INF_NAN");
	}
	if (fp_config & CL_FP_ROUND_TO_NEAREST) {
		fprintf(output, " ROUND_TO_NEAREST");
	}
	if (fp_config & CL_FP_ROUND_TO_ZERO) {
		fprintf(output, " ROUND_TO_ZERO");
	}
	if (fp_config & CL_FP_ROUND_TO_INF) {
		fprintf(output, " ROUND_TO_INF");
	}
	if (fp_config & CL_FP_FMA) {
		fprintf(output, " FMA");
	}
	fprintf(output, "\n");

	// double FP config
	cl_device_fp_config	double_fp_config;
	clGetDeviceInfo(device, CL_DEVICE_DOUBLE_FP_CONFIG,
		sizeof(double_fp_config), &double_fp_config, NULL);
	fprintf(output, "device double fp configuration:      ");
	if (double_fp_config & CL_FP_DENORM) {
		fprintf(output, " DENORM");
	}
	if (double_fp_config & CL_FP_INF_NAN) {
		fprintf(output, " INF_NAN");
	}
	if (double_fp_config & CL_FP_ROUND_TO_NEAREST) {
		fprintf(output, " ROUND_TO_NEAREST");
	}
	if (double_fp_config & CL_FP_ROUND_TO_ZERO) {
		fprintf(output, " ROUND_TO_ZERO");
	}
	if (double_fp_config & CL_FP_ROUND_TO_INF) {
		fprintf(output, " ROUND_TO_INF");
	}
	if (double_fp_config & CL_FP_FMA) {
		fprintf(output, " FMA");
	}
	fprintf(output, "\n");

	// mem cache type
	cl_device_mem_cache_type	mem_cache_type;
	clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
		sizeof(mem_cache_type), &mem_cache_type, NULL);
	fprintf(output, "device global mem cache type         ");
	switch (mem_cache_type) {
	case CL_NONE:
		fprintf(output, " NONE");
		break;
	case CL_READ_ONLY_CACHE:
		fprintf(output, " READ_ONLY_CACHE");
		break;
	case CL_READ_WRITE_CACHE:
		fprintf(output, " READ_WRITE_CACHE");
		break;
	}
	fprintf(output, "\n");

	// global mem cacheline size
	cl_uint	cacheline_size;
	clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
		sizeof(cacheline_size), &cacheline_size, NULL);
	fprintf(output, "device global mem cacheline size:     %u\n",
		cacheline_size);

	// device global mem cache size
	cl_ulong	global_mem_cache_size;
	clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
		sizeof(global_mem_cache_size), &global_mem_cache_size, NULL);
	fprintf(output, "device global mem cache size:         %lu\n",
		global_mem_cache_size);

	// global memory size
	cl_ulong	global_mem_size;
	clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE,
		sizeof(global_mem_size), &global_mem_size, NULL);
	fprintf(output, "device global mem size:               %lu\n",
		global_mem_size);

	// constant buffer size
	cl_ulong	constant_buffer_size;
	clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
		sizeof(constant_buffer_size), &constant_buffer_size, NULL);
	fprintf(output, "device max constant buffer size:      %lu\n",
		constant_buffer_size);

	// constant args
	cl_uint	constant_args;
	clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_ARGS,
		sizeof(constant_args), &constant_args, NULL);
	fprintf(output, "device max constant args:             %d\n",
		compute_units);

	// local mem type
	cl_device_local_mem_type	local_mem_type;
	clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE,
		sizeof(local_mem_type), &local_mem_type, NULL);
	fprintf(output, "device local memory type:             ");
	switch (local_mem_type) {
	case CL_LOCAL:
		fprintf(output, "LOCAL");
		break;
	case CL_GLOBAL:
		fprintf(output, "GLOBAL");
		break;
	}
	fprintf(output, "\n");

	// local mem size
	cl_ulong	local_mem_size;
	clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE,
		sizeof(local_mem_size), &local_mem_size, NULL);
	fprintf(output, "device local memory size:             %lu\n",
		local_mem_size);

	// device error correctio support
	cl_bool	error_correction_support;
	clGetDeviceInfo(device, CL_DEVICE_ERROR_CORRECTION_SUPPORT,
		sizeof(cl_bool), &error_correction_support, NULL);
	fprintf(output, "device error correction support:      %s\n",
		(error_correction_support) ? "yes" : "no");

	// profiling timer resolution
	size_t	profiling_timer;
	clGetDeviceInfo(device, CL_DEVICE_PROFILING_TIMER_RESOLUTION,
		sizeof(profiling_timer), &profiling_timer, NULL);
	fprintf(output, "device profiling timer resolution:    %ld\n",
		profiling_timer);

	// device endianity
	cl_bool	endian_little;
	clGetDeviceInfo(device, CL_DEVICE_ENDIAN_LITTLE, sizeof(cl_bool),
		&endian_little, NULL);
	fprintf(output, "device endianity:                     %s\n",
		(endian_little) ? "little" : "big");

	// availability
	cl_bool	available;
	clGetDeviceInfo(device, CL_DEVICE_AVAILABLE,
		sizeof(available), &available, NULL);
	fprintf(output, "device available:                     %s\n",
		(available) ? "yes" : "no");

	// device compiler available
	cl_bool	compiler_available;
	clGetDeviceInfo(device, CL_DEVICE_COMPILER_AVAILABLE,
		sizeof(compiler_available), &compiler_available, NULL);
	fprintf(output, "device compiler available:            %s\n",
		(compiler_available) ? "yes" : "no");

	// device execution capabilities
	cl_device_exec_capabilities	cap;
	clGetDeviceInfo(device, CL_DEVICE_EXECUTION_CAPABILITIES,
		sizeof(cap), &cap, NULL);
	fprintf(output, "device execution capabilities:       ");
	if (cap & CL_EXEC_KERNEL) {
		fprintf(output, " KERNEL");
	}
	if (cap & CL_EXEC_NATIVE_KERNEL) {
		fprintf(output, " NATIVE_KERNEL");
	}
	fprintf(output, "\n");

	// device queue properties
	cl_command_queue_properties	queue_properties;
	clGetDeviceInfo(device, CL_DEVICE_QUEUE_PROPERTIES,
		sizeof(queue_properties), &queue_properties, NULL);
	fprintf(output, "device queue properties:             ");
	if (queue_properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) {
		fprintf(output, " OUT_OF_ORDER_EXEC_MODE_ENABLE");
	}
	if (queue_properties & CL_QUEUE_PROFILING_ENABLE) {
		fprintf(output, " PROFILING_ENABLE");
	}
	fprintf(output, "\n");

	// device plattform
	cl_platform_id	platform;
	clGetDeviceInfo(device, CL_DEVICE_PLATFORM,
		sizeof(platform), &platform, NULL);
	fprintf(output, "device platform id:                   %p\n",
		platform);

	// device name
	char	buffer[1024];
	clGetDeviceInfo(device, CL_DEVICE_NAME,
		sizeof(buffer), buffer, NULL);
	fprintf(output, "device name:                          %s\n", buffer);

	// device vendor
	clGetDeviceInfo(device, CL_DEVICE_VENDOR,
		sizeof(buffer), buffer, NULL);
	fprintf(output, "device vendor:                        %s\n", buffer);

	// driver version
	clGetDeviceInfo(device, CL_DRIVER_VERSION,
		sizeof(buffer), buffer, NULL);
	fprintf(output, "device driver version:                %s\n", buffer);

	// device profile
	clGetDeviceInfo(device, CL_DEVICE_PROFILE,
		sizeof(buffer), buffer, NULL);
	fprintf(output, "device profile:                       %s\n", buffer);

	// device version
	clGetDeviceInfo(device, CL_DEVICE_VERSION,
		sizeof(buffer), buffer, NULL);
	fprintf(output, "device version:                       %s\n", buffer);

	// device extensions
	clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, sizeof(buffer), buffer,
		NULL);
	fprintf(output, "device extensions:                    %s\n", buffer);

}

/*
 * print an OpenCL error message
 */
void	clu_perror(cl_int err, const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
#define	CLU_PERROR_BUFFERSIZE	1024
	char	msg[CLU_PERROR_BUFFERSIZE];
	vsnprintf(msg, sizeof(msg), fmt, ap);
	va_end(ap);
	msg[CLU_PERROR_BUFFERSIZE - 1] = '\0';

	switch (err) {
	case CL_SUCCESS:
		fprintf(stderr, "%s: no error\n", msg);
		break;
	case CL_DEVICE_NOT_FOUND:
		fprintf(stderr, "%s: device not found (CL_DEVICE_NOT_FOUND)\n", msg);
		break;
	case CL_DEVICE_NOT_AVAILABLE:
		fprintf(stderr, "%s: device not available (CL_DEVICE_NOT_AVAILABLE)\n", msg);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		fprintf(stderr, "%s: enqueue: object allocation (CL_MEM_OBJECT_ALLOCATION_FAILURE)\n", msg);
		break;
	case CL_OUT_OF_RESOURCES:
		fprintf(stderr, "%s: enqueue: out of resources (CL_OUT_OF_RESOURCES)\n", msg);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		fprintf(stderr, "%s: enqueue: host memory (CL_OUT_OF_HOST_MEMORY)\n", msg);
		break;
	case CL_PROFILING_INFO_NOT_AVAILABLE:
		fprintf(stderr, "%s: profiling not available (CL_PROFILING_INFO_NOT_AVAILABLE)\n", msg);
		break;
	case CL_MEM_COPY_OVERLAP:
		fprintf(stderr, "%s: memory copy overlap (CL_MEM_COPY_OVERLAP)\n", msg);
		break;
	case CL_IMAGE_FORMAT_MISMATCH:
		fprintf(stderr, "%s: image format mismatch (CL_IMAGE_FORMAT_MISMATCH)\n", msg);
		break;
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
		fprintf(stderr, "%s: image format not supported (CL_IMAGE_FORMAT_NOT_SUPPORTED)\n", msg);
		break;
	case CL_BUILD_PROGRAM_FAILURE:
		fprintf(stderr, "%s: build program failure (CL_BUILD_PROGRAM_FAILURE)\n", msg);
		break;
	case CL_MAP_FAILURE:
		fprintf(stderr, "%s: map failure (CL_MAP_FAILURE)\n", msg);
		break;
	case CL_INVALID_VALUE:
		fprintf(stderr, "%s: invalid value (CL_INVALID_VALUE)\n", msg);
		break;
	case CL_INVALID_DEVICE_TYPE:
		fprintf(stderr, "%s: invalid device type (CL_INVALID_DEVICE_TYPE)\n", msg);
		break;
	case CL_INVALID_PLATFORM:
		fprintf(stderr, "%s: invalid platform (CL_INVALID_PLATFORM)\n", msg);
		break;
	case CL_INVALID_DEVICE:
		fprintf(stderr, "%s: invalid device (CL_INVALID_DEVICE)\n", msg);
		break;
	case CL_INVALID_CONTEXT:
		fprintf(stderr, "%s: invalid context (CL_INVALID_CONTEXT)\n", msg);
		break;
	case CL_INVALID_QUEUE_PROPERTIES:
		fprintf(stderr, "%s: invalid queue properties (CL_INVALID_QUEUE_PROPERTIES)\n", msg);
		break;
	case CL_INVALID_COMMAND_QUEUE:
		fprintf(stderr, "%s: invalid command queue (CL_INVALID_COMMAND_QUEUE)\n", msg);
		break;
	case CL_INVALID_HOST_PTR:
		fprintf(stderr, "%s: invalid host pointer (CL_INVALID_HOST_PTR)\n", msg);
		break;
	case CL_INVALID_MEM_OBJECT:
		fprintf(stderr, "%s: invalid memory object (CL_INVALID_MEM_OBJECT)\n", msg);
		break;
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
		fprintf(stderr, "%s: invalid image format descriptor (CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)\n", msg);
		break;
	case CL_INVALID_IMAGE_SIZE:
		fprintf(stderr, "%s: invalid image size (CL_INVALID_IMAGE_SIZE)\n", msg);
		break;
	case CL_INVALID_SAMPLER:
		fprintf(stderr, "%s: invalid samplern (CL_INVALID_SAMPLER)", msg);
		break;
	case CL_INVALID_BINARY:
		fprintf(stderr, "%s: invalid binary (CL_INVALID_BINARY)\n", msg);
		break;
	case CL_INVALID_BUILD_OPTIONS:
		fprintf(stderr, "%s: invalid build options (CL_INVALID_BUILD_OPTIONS)\n", msg);
		break;
	case CL_INVALID_PROGRAM:
		fprintf(stderr, "%s: invalid program (CL_INVALID_PROGRAM)\n", msg);
		break;
	case CL_INVALID_PROGRAM_EXECUTABLE:
		fprintf(stderr, "%s: invalid program executable (CL_INVALID_PROGRAM_EXECUTABLE)\n", msg);
		break;
	case CL_INVALID_KERNEL_NAME:
		fprintf(stderr, "%s: invalid kernel name (CL_INVALID_KERNEL_NAME)\n", msg);
		break;
	case CL_INVALID_KERNEL_DEFINITION:
		fprintf(stderr, "%s: invalid kernel definition (CL_INVALID_KERNEL_DEFINITION)\n", msg);
		break;
	case CL_INVALID_KERNEL:
		fprintf(stderr, "%s: invalid kernel (CL_INVALID_KERNEL)\n", msg);
		break;
	case CL_INVALID_ARG_INDEX:
		fprintf(stderr, "%s: invalid argument index (CL_INVALID_ARG_INDEX)\n", msg);
		break;
	case CL_INVALID_ARG_VALUE:
		fprintf(stderr, "%s: invalid argument value (CL_INVALID_ARG_VALUE)\n", msg);
		break;
	case CL_INVALID_ARG_SIZE:
		fprintf(stderr, "%s: invalid argument size (CL_INVALID_ARG_SIZE)\n", msg);
		break;
	case CL_INVALID_KERNEL_ARGS:
		fprintf(stderr, "%s: invalid kernel args (CL_INVALID_KERNEL_ARGS)\n", msg);
		break;
	case CL_INVALID_WORK_DIMENSION:
		fprintf(stderr, "%s: invalid work dimension (CL_INVALID_WORK_DIMENSION)\n", msg);
		break;
	case CL_INVALID_WORK_GROUP_SIZE:
		fprintf(stderr, "%s: invalid work group size (CL_INVALID_WORK_GROUP_SIZE)\n", msg);
		break;
	case CL_INVALID_WORK_ITEM_SIZE:
		fprintf(stderr, "%s: invalid work item size (CL_INVALID_WORK_ITEM_SIZE)\n", msg);
		break;
	case CL_INVALID_GLOBAL_OFFSET:
		fprintf(stderr, "%s: invalid global offset (CL_INVALID_GLOBAL_OFFSET)\n", msg);
		break;
	case CL_INVALID_EVENT_WAIT_LIST:
		fprintf(stderr, "%s: invalid event wait list (CL_INVALID_EVENT_WAIT_LIST)\n", msg);
		break;
	case CL_INVALID_EVENT:
		fprintf(stderr, "%s: invalid event (CL_INVALID_EVENT)\n", msg);
		break;
	case CL_INVALID_OPERATION:
		fprintf(stderr, "%s: invalid operation (CL_INVALID_OPERATION)\n", msg);
		break;
	case CL_INVALID_GL_OBJECT:
		fprintf(stderr, "%s: invalid gl operation (CL_INVALID_GL_OBJECT)\n", msg);
		break;
	case CL_INVALID_BUFFER_SIZE:
		fprintf(stderr, "%s: invalid buffer size (CL_INVALID_BUFFER_SIZE)\n", msg);
		break;
	case CL_INVALID_MIP_LEVEL:
		fprintf(stderr, "%s: invalid mip level (CL_INVALID_MIP_LEVEL)\n", msg);
		break;
	case CL_INVALID_GLOBAL_WORK_SIZE:
		fprintf(stderr, "%s: invalid global work size (CL_INVALID_GLOBAL_WORK_SIZE)\n", msg);
		break;
#ifdef CL_PLATFORM_NOT_FOUND_KHR
	case CL_PLATFORM_NOT_FOUND_KHR:
		fprintf(stderr, "%s: platform not found (CL_PLATFORM_NOT_FOUND_KHR)\n", msg);
		break;
#endif
#ifdef CL_DEVICE_PARTITION_FAILED_EXT
	case CL_DEVICE_PARTITION_FAILED_EXT:
		fprintf(stderr, "%s: device partition failed (CL_DEVICE_PARTITION_FAILED)\n", msg);
		break;
#endif
#ifdef CL_INVALID_PARTITION_COUNT_EXT
	case CL_INVALID_PARTITION_COUNT_EXT:
		fprintf(stderr, "%s: invalid partition count (CL_INVALID_PARTITION_COUNT_EXT)\n", msg);
		break;
#endif
#ifdef CL_INVALID_PARTITION_NAME_EXT
	case CL_INVALID_PARTITION_NAME_EXT:
		fprintf(stderr, "%s: invalid partition name (CL_INVALID_PARTITION_NAME_EXT)\n", msg);
		break;
#endif
	default:
		fprintf(stderr, "%s: unknown error code %d\n", msg, err);
		break;
	}
	fflush(stderr);
}

/*
 * data structures and methods to create a compiler
 */
typedef struct define_entry_s {
	char *key;
	char *value;
	struct define_entry_s	*next;
} define_entry_t;

static void	define_delete(define_entry_t *entry) {
	free(entry->key);
	free(entry->value);
	free(entry);
}

static void	defines_delete(define_entry_t *entry) {
	if (entry->next) {
		defines_delete(entry->next);
	}
	define_delete(entry);
}

typedef struct cl_private_s {
	define_entry_t	*defines;
} cl_private_t;

/*
 * create a new compiler structure
 */
clcompiler_t	*clu_newcompiler(cl_context context, cl_device_id device_id) {
	clcompiler_t	*result = (clcompiler_t *)malloc(sizeof(clcompiler_t));
	result->debug = 0;
	result->context = context;
	result->device_id = device_id;
	cl_private_t	*priv = malloc(sizeof(cl_private_t));
	priv->defines = NULL;
	result->private = priv;
	return result;
}

/*
 * add a define to the compiler command line
 */
void	clu_define(clcompiler_t *compiler,
		const char *name, const char *value) {
	if (debug) {
		printf("%s:%d: adding -D %s=%s\n", __FILE__, __LINE__,
			name, value);
	}
	define_entry_t	*entry = (define_entry_t *)malloc(sizeof(define_entry_t));

	entry->key = strdup(name);
	entry->value = strdup(value);
	entry->next = NULL;

	cl_private_t	*priv = compiler->private;
	if (NULL == priv->defines) {
		priv->defines = entry;
	} else {
		define_entry_t	*p = priv->defines;
		while (p->next) {
			p = p->next;
		}
		p->next = entry;
	}
}

void	clu_vdefine(clcompiler_t *compiler,
		const char *name, const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	char	buffer[1024];
	vsnprintf(buffer, sizeof(buffer), fmt, ap);
	va_end(ap);
	clu_define(compiler, name, buffer);
}

static char	*clu_search(clcompiler_t *compiler, const char *programname) {
	char	*result = NULL;
	char	buffer[1024];
	if (debug) {
		printf("%s:%d: searching for kernel named %s\n",
			__FILE__, __LINE__, programname);
	}
	if (compiler->path) {
		snprintf(buffer, sizeof(buffer), "%s/%s", compiler->path,
			programname);
		if (0 == access(buffer, R_OK)) {
			if (debug) {
				printf("%s:%d: found %s\n", __FILE__, __LINE__,
					buffer);
			}
			return strdup(buffer);
		}
	}
	snprintf(buffer, sizeof(buffer), "%s/%s", PKGDATADIR, programname);
	if (0 == access(buffer, R_OK)) {
		if (debug) {
			printf("%s:%d: found %s\n", __FILE__, __LINE__,
				buffer);
		}
		return strdup(buffer);
	}
	return result;
}

/*
 * the compiler program
 */
int	clu_compile(clcompiler_t *compiler, char *kernelfile) {
	char	cmdline[10240];
	int	offset = 0;
	int	bytes;
	cl_int	err;

	/* create the program */
	char	*kernelpath = clu_search(compiler, kernelfile);
	if (NULL == kernelpath) {
		errno = ENOENT;
		return -1;
	}
	if (debug) {
		printf("%s:%d: kernel path is '%s'\n", __FILE__, __LINE__,
			kernelpath);
	}
	compiler->program = cluCreateProgramWithFilename(compiler->context,
		kernelpath, &err);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot create program");
		free(kernelpath);
		return -1;
        }
	if (debug) {
		printf("%s:%d: program created from path %s\n",
			__FILE__, __LINE__, kernelpath);
	}
	free(kernelpath);

	/* now start building the kernel command line */
	/* debug flag */
	if (compiler->debug) {
		bytes = snprintf(&cmdline[offset], sizeof(cmdline) - offset,
			"-D DEBUG=1");
		if (bytes > (sizeof(cmdline) - offset)) {
			errno = ENOSPC;
			return -1;
		}
		offset += bytes;
	}

	/* include path for cl files, first the one from the -P option */
	if (compiler->path) {
#if 0
		bytes = snprintf(&cmdline[offset], sizeof(cmdline) - offset, 
			" -I \"%s\"", compiler->path);
#endif
		bytes = snprintf(&cmdline[offset], sizeof(cmdline) - offset, 
			" -I %s", compiler->path);
		if (bytes > (sizeof(cmdline) - offset)) {
			errno = ENOSPC;
			return -1;
		}
		offset += bytes;
	}
	/* include path from the pkgdatadir */
	bytes = snprintf(&cmdline[offset], sizeof(cmdline) - offset, 
		" -I %s", PKGDATADIR);
	if (bytes > (sizeof(cmdline) - offset)) {
		errno = ENOSPC;
		return -1;
	}
	offset += bytes;

	/* add all entries in the defines list as defines */
	cl_private_t	*priv = (cl_private_t *)compiler->private;
	define_entry_t	*entry = priv->defines;
	while (entry) {
		bytes = snprintf(&cmdline[offset], sizeof(cmdline) - offset,
			" -D %s=%s",
			entry->key, entry->value);
		if (bytes > (sizeof(cmdline) - offset)) {
			errno = ENOSPC;
			return -1;
		}
		offset += bytes;
		entry = entry->next;
	}
	if (debug) {
		printf("%s:%d: compiler command line: %s\n", __FILE__, __LINE__,
			cmdline);
	}
	/* now the command line has been constructed, and we can continue
	   by compiling the program */
	err = clBuildProgram(compiler->program, 0, NULL, cmdline, NULL, NULL);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot build program");
		size_t  len = 0;
		char    *buffer;
		clGetProgramBuildInfo(compiler->program, compiler->device_id,
			CL_PROGRAM_BUILD_LOG, len, NULL, &len);
		fprintf(stderr, "build log length: %ld\n", len);
		buffer = (char *)malloc(len + 1);
		clGetProgramBuildInfo(compiler->program, compiler->device_id,
			CL_PROGRAM_BUILD_LOG, len + 1, buffer, &len);
		fprintf(stderr, "cannot build a program (%d):\n%s\n",
			err, buffer);
		free(buffer);
		return -1;
	}

	/* compilation was successful, so we can now build a kernel */
	compiler->eliminate = clCreateKernel(compiler->program, "eliminate", &err);
        if (CL_SUCCESS != err) {
                clu_perror(err, "cannot create eliminate kernel");
                return -1;
        }
	compiler->dividerow = clCreateKernel(compiler->program, "dividerow", &err);
        if (CL_SUCCESS != err) {
                clu_perror(err, "cannot create dividerow kernel");
                return -1;
        }
        if (debug) {
                printf("%s:%d: kernel created\n", __FILE__, __LINE__);
        }

	return 0;
}

void	clu_freecompiler(clcompiler_t *compiler) {
	if (NULL == compiler) {
		return;
	}
	cl_private_t	*priv = (cl_private_t *)compiler->private;
	if (priv) {
		if (priv->defines) {
			defines_delete(priv->defines);
			priv->defines = NULL;
		}
		free(priv);
		compiler->private = NULL;
	}
	clReleaseKernel(compiler->eliminate);
	clReleaseKernel(compiler->dividerow);
	clReleaseProgram(compiler->program);
	free(compiler);
}

clu_platform_t	*clu_getplatform(int platform) {
	// retrieve the platform ids
	cl_platform_id	platformids[10];
	cl_uint	nplatformids;
	cl_int	err = clGetPlatformIDs(10, platformids, &nplatformids);
	if (CL_SUCCESS != err) {
		fprintf(stderr, "clGetPlatformIDs failed. %d\n", err);
		return NULL;
	}

	// show info about all platforms
	if (debug) {
		for (int i = 0; i < nplatformids; i++) {
			fprintf(stdout, "platform number: %d\n", i);
			clu_showplatforminfo(stdout, platformids[i]);
		}
	}

	// retrieve the device ids, always select platform 0
	clu_platform_t	*result = (clu_platform_t *)malloc(
		sizeof(clu_platform_t));
	result->platformid = platformids[platform];

	err = clGetDeviceIDs(result->platformid,
		(usegpu) ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU,
		4, result->device_id, &result->ndeviceids);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot retrieve device ID");
		free(result);
		return NULL;
	}

	// display the information about the devices
	if (debug) {
		printf("%s:%d: ndeviceids = %d\n", __FILE__, __LINE__,
			result->ndeviceids);
		for (int i = 0; i < result->ndeviceids; i++) {
			printf("device %d:\n", i);
			clu_showdeviceinfo(stdout, result->device_id[i]);
		}
	}

	// create a context
	// XXX because someone else is also using the graphics card,
	//     for the time being, we have to restrict to a single card
	//result->ndeviceids = 1;
	result->context = clCreateContext(0, result->ndeviceids,
		result->device_id, NULL, NULL, &err);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot create OpenCL context\n");
		free(result);
		return NULL;
	}
	if (debug) {
		fprintf(stderr, "%s:%d: context created\n", __FILE__, __LINE__);
	}

	return result;
}

void	clu_releaseplatform(clu_platform_t *platform) {
	clReleaseContext(platform->context);
	free(platform);
}

clu_device_t	*clu_getdevice(clu_platform_t *platform, int device) {
	clu_device_t	*result = (clu_device_t *)malloc(sizeof(clu_device_t));
	result->platform = platform;
	result->device = device;
	result->device_id = platform->device_id[device];

	// create a command queue
	cl_int	err;
	result->queue = clCreateCommandQueue(platform->context,
		result->device_id, 0, &err);
	if (err != CL_SUCCESS) {
		clu_perror(err, "cannot create command queue\n");
		free(result);
		return NULL;
	}

	return result;
}

void	clu_releasedevice(clu_device_t *device) {
	clReleaseCommandQueue(device->queue);
	free(device);
}
