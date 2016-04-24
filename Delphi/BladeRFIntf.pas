unit BladeRFIntf;

interface

uses
  Windows;

type
  bladerf_module = (
    BLADERF_MODULE_INVALID = -1,    //**< Invalid module entry */
    BLADERF_MODULE_RX,              //**< Receive Module */
    BLADERF_MODULE_TX               //**< Transmit Module */
  );

  bladerf_format = (
    {**
     * Signed, Complex 16-bit Q11. This is the native format of the DAC data.
     *
     * Values in the range [-2048, 2048) are used to represent [-1.0, 1.0).
     * Note that the lower bound here is inclusive, and the upper bound is
     * exclusive. Ensure that provided samples stay within [-2048, 2047].
     *
     * Samples consist of interleaved IQ value pairs, with I being the first
     * value in the pair. Each value in the pair is a right-aligned,
     * little-endian int16_t. The FPGA ensures that these values are
     * sign-extended.
     *
     * When using this format the minimum required buffer size, in bytes, is:
     * <pre>
     *   buffer_size_min = [ 2 * num_samples * sizeof(int16_t) ]
     * </pre>
     *
     * For example, to hold 2048 samples, a buffer must be at least 8192 bytes
     * large.
     *}
    BLADERF_FORMAT_SC16_Q11,

    {**
     * This format is the same as the ::BLADERF_FORMAT_SC16_Q11 format, except the
     * first 4 samples (16 bytes) in every block of 1024 samples are replaced
     * with metadata, organized as follows, with all fields being little endian
     * byte order:
     *
     * <pre>
     *  0x00 [uint32_t:  Reserved]
     *  0x04 [uint64_t:  64-bit Timestamp]
     *  0x0c [uint32_t:  BLADERF_META_FLAG_* flags]
     * </pre>
     *
     * When using the bladerf_sync_rx() and bladerf_sync_tx() functions,
     * this detail is transparent to caller. These functions take care of
     * packing/unpacking the metadata into/from the data, via the
     * bladerf_metadata structure.
     *
     * Currently, when using the asynchronous data transfer interface, the user
     * is responsible for manually packing/unpacking this metadata into/from
     * their sample data.
     *}
    BLADERF_FORMAT_SC16_Q11_META
  );

  bladerf_metadata = record
    {**
     * Free-running FPGA counter that monotonically increases at the
     * sample rate of the associated module. *}
    timestamp : array[0..7] of Byte;

    {**
     * Input bit field to control the behavior of the call that the metadata
     * structure is passed to. API calls read this field from the provided
     * data structure, and do not modify it.
     *
     * Valid flags include
     *  ::BLADERF_META_FLAG_TX_BURST_START, ::BLADERF_META_FLAG_TX_BURST_END,
     *  ::BLADERF_META_FLAG_TX_NOW, ::BLADERF_META_FLAG_TX_UPDATE_TIMESTAMP,
     *  and ::BLADERF_META_FLAG_RX_NOW
     *
     *}
    flags : array[0..3] of byte;

    {**
     * Output bit field to denoting the status of transmissions/receptions. API
     * calls will write this field.
     *
     * Possible status flags include ::BLADERF_META_STATUS_OVERRUN and
     * ::BLADERF_META_STATUS_UNDERRUN;
     *
     *}
    status : array[0..4] of byte;

    {**
     * This output parameter is updated to reflect the actual number of
     * contiguous samples that have been populated in an RX buffer during
     * a bladerf_sync_rx() call.
     *
     * This will not be equal to the requested count in the event of a
     * discontinuity (i.e., when the status field has the
     * ::BLADERF_META_STATUS_OVERRUN flag set). When an overrun occurs, it is
     * important not to read past the number of samples specified by this
     * value, as the remaining contents of the buffer are undefined.
     *
     * This parameter is not currently used by bladerf_sync_tx().
     *}
    actual_count : array[0..3] of byte;

    {**
     * Reserved for future use. This is not used by any functions.
     * It is recommended that users zero out this field.
     *}
    reserved : array[0..31] of byte;
  end;

  pbladerf_metadata = ^bladerf_metadata;




function bladerf_open(var device : Pointer;
                      device_identifier: PChar): Integer; cdecl; external 'bladeRF.dll';
function bladerf_strerror(error: Integer): PChar; cdecl; external 'bladeRF.dll';
procedure bladerf_close(device: Pointer); cdecl; external 'bladeRF.dll';
function bladerf_set_frequency(device: Pointer;
                               module: bladerf_module;
                               frequency: UINT): Integer; cdecl; external 'bladeRF.dll';
function bladerf_get_frequency(device: Pointer;
                               module: bladerf_module;
                               var frequency: UINT): Integer; cdecl; external 'bladeRF.dll';
function bladerf_set_sample_rate(device: Pointer;
                                 module: bladerf_module;
                                 rate: UINT;
                                 var actual: UINT): Integer; cdecl; external 'bladeRF.dll';
function bladerf_set_bandwidth(device: Pointer;
                               module: bladerf_module;
                               bandwidth: UINT;
                               var actual: UINT): Integer; cdecl; external 'bladeRF.dll';
function bladerf_enable_module(device: Pointer;
                               module: bladerf_module;
                               enable: Boolean): Integer; cdecl; external 'bladeRF.dll';
function bladerf_sync_config(device : Pointer;
                             module : bladerf_module;
                             format : bladerf_format;
                             num_buffers : UINT;
                             buffer_size : UINT;
                             num_transfers : UINT;
                             stream_timeout : UINT) : Integer;  cdecl; external 'bladeRF.dll';
function bladerf_sync_rx(device : Pointer;
                         samples : Pointer;
                         num_samples : UINT;
                         metadata : pbladerf_metadata;
                         timeout_ms : UINT) : Integer;  cdecl; external 'bladeRF.dll';

implementation


end.
 