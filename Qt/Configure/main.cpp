//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//#include <libbladeRF.h>

//// The RX and TX modules are configured independently for these parameters
//struct module_config {
//    bladerf_module module;
//    unsigned int frequency;
//    unsigned int bandwidth;
//    unsigned int samplerate;
//    /* Gains */
//    bladerf_lna_gain rx_lna;
//    int vga1;
//    int vga2;
//};

//int configure_module(struct bladerf *dev, struct module_config *c) {
//    int status;
//	unsigned int freq;
//	status = bladerf_get_frequency(dev, c->module, &freq);
//    if (status != 0) {
//        fprintf(stderr, "Failed to get frequency: %s\n", bladerf_strerror(status));
//        return status;
//    };
//    fprintf(stderr, "Current frequency = %u\n", c->frequency);
//    status = bladerf_set_frequency(dev, c->module, c->frequency);
//    if (status != 0) {
//        fprintf(stderr, "Failed to set frequency = %u: %s\n", c->frequency, bladerf_strerror(status));
//        return status;
//    };
//    status = bladerf_set_sample_rate(dev, c->module, c->samplerate, NULL);
//    if (status != 0) {
//        fprintf(stderr, "Failed to set samplerate = %u: %s\n", c->samplerate, bladerf_strerror(status));
//        return status;
//    };
//    status = bladerf_set_bandwidth(dev, c->module, c->bandwidth, NULL);
//    if (status != 0) {
//        fprintf(stderr, "Failed to set bandwidth = %u: %s\n", c->bandwidth, bladerf_strerror(status));
//        return status;
//    };
//    switch (c->module) {
//        case BLADERF_MODULE_RX:
//            /* Configure the gains of the RX LNA, RX VGA1, and RX VGA2  */
//            status = bladerf_set_lna_gain(dev, c->rx_lna);
//            if (status != 0) {
//                fprintf(stderr, "Failed to set RX LNA gain: %s\n", bladerf_strerror(status));
//                return status;
//            }
//            status = bladerf_set_rxvga1(dev, c->vga1);
//            if (status != 0) {
//                fprintf(stderr, "Failed to set RX VGA1 gain: %s\n", bladerf_strerror(status));
//                return status;
//            }
//            status = bladerf_set_rxvga2(dev, c->vga2);
//            if (status != 0) {
//                fprintf(stderr, "Failed to set RX VGA2 gain: %s\n", bladerf_strerror(status));
//                return status;
//            };
//            break;
//        case BLADERF_MODULE_TX:
//            /* Configure the TX VGA1 and TX VGA2 gains */
//            status = bladerf_set_txvga1(dev, c->vga1);
//            if (status != 0) {
//                fprintf(stderr, "Failed to set TX VGA1 gain: %s\n", bladerf_strerror(status));
//                return status;
//            };
//            status = bladerf_set_txvga2(dev, c->vga2);
//            if (status != 0) {
//                fprintf(stderr, "Failed to set TX VGA2 gain: %s\n", bladerf_strerror(status));
//                return status;
//            };
//            break;
//        default:
//            status = BLADERF_ERR_INVAL;
//            fprintf(stderr, "%s: Invalid module specified (%d)\n", __FUNCTION__, c->module);
//    }
//    return status;
//}

///* Usage:
// *   libbladeRF_example_boilerplate [serial #]
// *
// * If a serial number is supplied, the program will attempt to open the
// * device with the provided serial number.
// *
// * Otherwise, the first available device will be used.
// */
//int main(int argc, char *argv[]) {
//	int status = 0;
//	struct module_config config;
//	struct bladerf *dev = NULL;
//	struct bladerf_devinfo dev_info;

//	struct bladerf_devinfo * devices;
//	// Initialize the information used to identify the desired device
//	// to all wildcard (i.e., "any device") values
//	bladerf_init_devinfo(&dev_info);
//	// Request a device with the provided serial number.
//	//  Invalid strings should simply fail to match a device.
//	if (argc >= 2) {
//		strncpy(dev_info.serial, argv[1], sizeof(dev_info.serial) - 1);
//	};
//	status = bladerf_get_device_list(&devices);
//	if (status <= 0) {
//		fprintf(stderr, "Unable to get device list: %s\n", bladerf_strerror(status));
//		return 1;
//	};
//	strncpy(dev_info.serial, devices[0].serial, BLADERF_SERIAL_LENGTH);
//	status = bladerf_open_with_devinfo(&dev, &devices[0]);
//	if (status != 0) {
//		fprintf(stderr, "Unable to open device: %s\n", bladerf_strerror(status));
//		return 1;
//	};
//    // Set up RX module parameters
//    config.module     = BLADERF_MODULE_RX;
//    config.frequency  = 910000000;
//    config.bandwidth  = 1500000;
//    config.samplerate = 2000000;
//    config.rx_lna     = BLADERF_LNA_GAIN_MAX;
//    config.vga1       = 30;
//    config.vga2       = 3;
//    status = configure_module(dev, &config);
//    if (status != 0) {
//        fprintf(stderr, "Failed to configure RX module. Exiting.\n");
//        goto out;
//    };
//    /* Set up TX module parameters */
//    config.module     = BLADERF_MODULE_TX;
//    config.frequency  = 918000000;
//    config.bandwidth  = 1500000;
//    config.samplerate = 250000;
//    config.vga1       = -14;
//    config.vga2       = 0;
//    status = configure_module(dev, &config);
//    if (status != 0) {
//        fprintf(stderr, "Failed to configure TX module. Exiting.\n");
//        goto out;
//    };
//    /* Application code goes here.
//     *
//     * Don't forget to call bladerf_enable_module() before attempting to
//     * transmit or receive samples!
//     */
//out:
//    bladerf_close(dev);
//	return status;
//}


#include <iostream>
#include <fstream>
#include <cstdlib>


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include "libbladeRF.h"

using namespace std;

static int init_sync(struct bladerf *dev) {
	int status;
	/* These items configure the underlying asynch stream used by the sync
	 * interface. The "buffer" here refers to those used internally by worker
	 * threads, not the user's sample buffers.
	 *
	 * It is important to remember that TX buffers will not be submitted to
	 * the hardware until `buffer_size` samples are provided via the
	 * bladerf_sync_tx call.  Similarly, samples will not be available to
	 * RX via bladerf_sync_rx() until a block of `buffer_size` samples has been
	 * received.
	 */
	const unsigned int num_buffers   = 32;
	const unsigned int buffer_size   = 32768;  /* Must be a multiple of 1024 */
	const unsigned int num_transfers = 16;
	const unsigned int timeout_ms    = 3500;
	/* Configure both the device's RX and TX modules for use with the synchronous
	 * interface. SC16 Q11 samples *without* metadata are used. */
	status = bladerf_sync_config(dev,
								 BLADERF_MODULE_RX,
								 BLADERF_FORMAT_SC16_Q11,
								 num_buffers,
								 buffer_size,
								 num_transfers,
								 timeout_ms);
	if (status != 0) {
		fprintf(stderr, "Failed to configure RX sync interface: %s\n",
				bladerf_strerror(status));
		return status;
	}
	status = bladerf_sync_config(dev,
								 BLADERF_MODULE_TX,
								 BLADERF_FORMAT_SC16_Q11,
								 num_buffers,
								 buffer_size,
								 num_transfers,
								 timeout_ms);
	if (status != 0) {
		fprintf(stderr, "Failed to configure TX sync interface: %s\n",
				bladerf_strerror(status));
	}
	return status;
}

int sync_rx_example(struct bladerf *dev) {
	int status, ret;
	bool done = false;
	//bool have_tx_data = false;
	/* "User" samples buffers and their associated sizes, in units of samples.
	 * Recall that one sample = two int16_t values. */
	int16_t *rx_samples = NULL;
	int16_t *tx_samples = NULL;
	const unsigned int samples_len = 1024*5000; /* May be any (reasonable) size */
	const unsigned int rx_size = samples_len*sizeof(int16_t)*2; /* May be any (reasonable) size */
	/* Allocate a buffer to store received samples in */
	//rx_samples = (int16_t*)malloc(samples_len * 2 * sizeof(int16_t));
	rx_samples = (int16_t*)malloc(rx_size);
	memset(rx_samples, 0, rx_size);
	if (rx_samples == NULL) {
		perror("malloc");
		return BLADERF_ERR_MEM;
	}
	/* Allocate a buffer to prepare transmit data in */
	tx_samples = (int16_t*)malloc(samples_len * 2 * sizeof(int16_t));
	if (tx_samples == NULL) {
		perror("malloc");
		free(rx_samples);
		return BLADERF_ERR_MEM;
	}
	/* Initialize synch interface on RX and TX modules */
	status = init_sync(dev);
	if (status != 0) {
		return -1;
	}
	status = bladerf_enable_module(dev, BLADERF_MODULE_RX, true);
	if (status != 0) {
		fprintf(stderr, "Failed to enable RX module: %s\n",
				bladerf_strerror(status));
		return -1;
	}
	status = bladerf_enable_module(dev, BLADERF_MODULE_TX, false);
	if (status != 0) {
		fprintf(stderr, "Failed to enable RX module: %s\n",
				bladerf_strerror(status));
		return -1;
	}

	unsigned int freq;
	status = bladerf_get_frequency(dev, BLADERF_MODULE_RX, &freq);
	if(status) {
		printf("failed: %s\n", bladerf_strerror(status));
		bladerf_close(dev);
		return 1;
	}
	printf("Tuned to %u\n", freq);


	// Открываем файл для записи
	std::ofstream ofile("d:/bladerf.dat", std::ios::binary);

	int count = 0;
	while (status == 0 && !done) {
		/* Receive samples */
		status = bladerf_sync_rx(dev, rx_samples, samples_len, NULL, 5000);
		if (status == 0) {
			// Пишем в файл
			int16_t * rx_samples2 = (int16_t*)malloc(rx_size / 2);
			UINT32 i = 0;
			int k = 0;
			while (i < rx_size / 2) {
				int16_t val = rx_samples[i] + rx_samples[i + 1];
				//int16_t val = rx_samples[i];
//				val &= val & 0x0fff;
//				if (val & 0x800) {
//				   val |= 0xf000;
//				};
				rx_samples2[k] = val;
				k++;
				i += 2;
			};
			ofile.write((char *)rx_samples2, rx_size / 2);
//			ofile.write((char *)rx_samples, rx_size);
//			usleep(20000);
			/* Process these samples, and potentially produce a response
			 * to transmit */
//			done = do_work(rx_samples, samples_len,
//						   &have_tx_data, tx_samples, samples_len);
//			if (!done && have_tx_data) {
				/* Transmit a response */
//				status = bladerf_sync_tx(dev, rx_samples, samples_len, NULL, 5000);
//				if (status != 0) {
//					fprintf(stderr, "Failed to TX samples: %s\n",
//							bladerf_strerror(status));
//				}
			//}
		} else {
			fprintf(stderr, "Failed to RX samples: %s\n",
					bladerf_strerror(status));
		}
		break;
		count++;
		if (count >= 1000)
			break;
	};
	ofile.close();
//	if (status == 0) {
//		/* Wait a few seconds for any remaining TX samples to finish
//		 * reaching the RF front-end */
//		usleep(2000000);
//	}


	ret = status;
	/* Disable RX module, shutting down our underlying RX stream */
	status = bladerf_enable_module(dev, BLADERF_MODULE_RX, false);
	if (status != 0) {
		fprintf(stderr, "Failed to disable RX module: %s\n",
				bladerf_strerror(status));
	}
	/* Disable TX module, shutting down our underlying TX stream */
	status = bladerf_enable_module(dev, BLADERF_MODULE_TX, false);
	if (status != 0) {
		fprintf(stderr, "Failed to disable TX module: %s\n",
				bladerf_strerror(status));
	}
	/* Free up our resources */
	free(rx_samples);
	free(tx_samples);
	return ret;
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	struct bladerf *dev;
	int status;

	printf("Connecting to device... ");
	fflush(stdout);
	status = bladerf_open(&dev, NULL);
	if (status) {
		printf("failed: %s\n", bladerf_strerror(status));
		bladerf_close(dev);
		return 1;
	};
	printf("OK\n");

//	printf("Enabling TX... ");
//	fflush(stdout);
//	status = bladerf_enable_module(dev, BLADERF_MODULE_TX, true);
//	if(status) {
//		printf("failed: %s\n", bladerf_strerror(status));
//		bladerf_close(dev);
//		return 1;
//	}
//	printf("OK\n");

	unsigned int rx_freq = 434925000;
	unsigned int tx_freq = 435925000;
	printf("Tuning TX to %uHz... \n", tx_freq);
	fflush(stdout);
	status = bladerf_set_frequency(dev, BLADERF_MODULE_TX, tx_freq);
	if(status) {
		printf("failed: %s\n", bladerf_strerror(status));
		bladerf_close(dev);
		return 1;
	};
	status = bladerf_get_frequency(dev, BLADERF_MODULE_TX, &tx_freq);
	if(status) {
		printf("failed: %s\n", bladerf_strerror(status));
		bladerf_close(dev);
		return 1;
	};
	printf("TX tuned to %u\n", tx_freq);

	/* RX */
	printf("Tuning RX to %uHz... \n", rx_freq);
	fflush(stdout);
	status = bladerf_set_frequency(dev, BLADERF_MODULE_RX, rx_freq);
	if (status) {
		printf("failed: %s\n", bladerf_strerror(status));
		bladerf_close(dev);
		return 1;
	};
	status = bladerf_get_frequency(dev, BLADERF_MODULE_RX, &rx_freq);
	if (status) {
		printf("failed: %s\n", bladerf_strerror(status));
		bladerf_close(dev);
		return 1;
	};
	printf("RX tuned to %u\n", rx_freq);


	unsigned int rx_sr = 4000000;
	printf("Setting RX sampling rate to %9usps... \n", rx_sr);
	fflush(stdout);
	unsigned int actual_rx_sr;
	status = bladerf_set_sample_rate(dev, BLADERF_MODULE_RX, rx_sr, &actual_rx_sr);
	if (status) {
		printf("failed: %s\n", bladerf_strerror(status));
		bladerf_close(dev);
		return 1;
	};
	printf("Actual RX sampling rate:    %9usps\n", actual_rx_sr);

	unsigned int rx_bw = 1500000;
	unsigned int actual_rx_bw;
	printf("Setting RX bandwidth to %9uHz... \n", rx_bw);
	fflush(stdout);
	status = bladerf_set_bandwidth(dev, BLADERF_MODULE_RX, rx_bw, &actual_rx_bw);
	if (status) {
		printf("failed: %s\n", bladerf_strerror(status));
		bladerf_close(dev);
		return 1;
	};
	printf("Actual RX bandwidth:    %9uHz\n", actual_rx_bw);

//	printf("Setting post-LPF gain to 0dB... ");
//	fflush(stdout);
//	status = bladerf_set_txvga1(dev, 0);
//	if(status) {
//		printf("failed: %s\n", bladerf_strerror(status));
//		bladerf_close(dev);
//		return 1;
//	}
//	printf("OK\n");

//	printf("Setting PA gain to 0dB... ");
//	fflush(stdout);
//	status = bladerf_set_txvga2(dev, 0);
//	if(status) {
//		printf("failed: %s\n", bladerf_strerror(status));
//		bladerf_close(dev);
//		return 1;
//	}
//	printf("OK\n");

	printf("All set up. Generating signals... \n");
	fflush(stdout);


	sync_rx_example(dev);

//	int samples_per_tx = 262144;
//	samples_per_tx = 32768;
//	int seconds_to_tx = 3;
//	int n_tx = ((seconds_to_tx * actual_sr) / samples_per_tx) + 1;
//	int nsamples = n_tx * samples_per_tx;
//	printf("%i transmissions, %i samples... ", n_tx, nsamples);
//	//int16_t samples[2 * nsamples];
//	signed short samples[2 * nsamples];
//	int n;
//	double t;
//	for (n = 0; n < (2 * nsamples); n += 2) {
//		t = ((double)(n / 2)) / ((double)actual_sr);
//		if ((n/2) % (samples_per_tx * 2) < samples_per_tx) {
//			samples[n  ] = 0x0fff & (int16_t)(2048.0 * cos(2.0 * M_PI * 1300.0 * t));
//			samples[n+1] = 0x0fff & (int16_t)(2048.0 * sin(2.0 * M_PI * 1300.0 * t));
//		} else {
//			samples[n  ] = 0x0fff & (int16_t)(2048.0 * cos(2.0 * M_PI * 700.0 * t));
//			samples[n+1] = 0x0fff & (int16_t)(2048.0 * sin(2.0 * M_PI * 700.0 * t));
//		}
//	}
//	printf("OK\n");

//	printf("Transmitting... ");
//	fflush(stdout);
//	int i;
//	for(i = 0; i < n_tx; i++) {
//		printf("tx %i... ", i);
//		fflush(stdout);
//		signed short * start_sample = &samples[i * samples_per_tx];
//		status = bladerf_sync_tx(dev, BLADERF_FORMAT_SC16_Q11, start_sample, samples_per_tx, NULL);
//		if(status < 0) {
//			printf("failed: %s\n", bladerf_strerror(status));
//			bladerf_close(dev);
//			return 1;
//		}
//	}
	printf("OK\n");

	printf("Closing down... ");
	bladerf_close(dev);
	printf("OK\n");
	return 0;
}
