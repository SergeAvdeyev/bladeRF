#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: FRS Transceiver
# Author: Nuand, LLC <bladeRF@nuand.com>
# Description: Multi-channel FRS transceiver with CTCSS.  A USB 3.0 connection is required for full-duplex operation.
# Generated: Thu May 19 21:44:13 2016
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import analog
from gnuradio import audio
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.filter import pfb
from gnuradio.qtgui import Range, RangeWidget
from grc_gnuradio import blks2 as grc_blks2
from optparse import OptionParser
import osmosdr
import sip
import sys
import time


class bladeRF_frs(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "FRS Transceiver")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("FRS Transceiver")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "bladeRF_frs")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.tx_audio_sample_rate = tx_audio_sample_rate = 25000
        self.tx_rf_freq = tx_rf_freq = 465.1375e6-200e3
        self.tx_pfs_output_rate = tx_pfs_output_rate = 16 * tx_audio_sample_rate
        self.tx_ch9_enable = tx_ch9_enable = True
        self.tx_ch8_enable = tx_ch8_enable = False
        self.tx_ch7_enable = tx_ch7_enable = False
        self.tx_ch6_enable = tx_ch6_enable = False
        self.tx_ch5_enable = tx_ch5_enable = False
        self.tx_ch4_enable = tx_ch4_enable = False
        self.tx_ch3_enable = tx_ch3_enable = False
        self.tx_ch2_enable = tx_ch2_enable = False
        self.tx_ch1_enable = tx_ch1_enable = False
        self.tx_ch14_enable = tx_ch14_enable = False
        self.tx_ch13_enable = tx_ch13_enable = False
        self.tx_ch12_enable = tx_ch12_enable = False
        self.tx_ch11_enable = tx_ch11_enable = False
        self.tx_ch10_enable = tx_ch10_enable = False
        self.rx_audio_sample_rate = rx_audio_sample_rate = int(25e3)
        self.rf_rx_freq = rf_rx_freq = 465.1375e6 + 200e3
        self.ctcss_freq = ctcss_freq = 0
        self.tx_vga2_gain = tx_vga2_gain = 10
        self.tx_vga1_gain = tx_vga1_gain = -8
        self.tx_rf_sample_rate = tx_rf_sample_rate = 20 * tx_pfs_output_rate
        self.tx_ptt = tx_ptt = False
        self.tx_pfs_taps = tx_pfs_taps = [-6.3717e-06,-7.2608e-06,-1.0952e-05,-1.543e-05,-2.0595e-05,-2.6243e-05,-3.2061e-05,-3.7629e-05,-4.2397e-05,-4.5735e-05,-4.692e-05,-4.5194e-05,-3.9798e-05,-3.0033e-05,-1.5328e-05,4.699e-06,3.016e-05,6.0834e-05,9.6104e-05,0.00013491,0.00017572,0.00021655,0.00025495,0.00028812,0.00031298,0.00032628,0.00032482,0.00030564,0.00026619,0.0002046,0.00011989,1.2225e-05,-0.00011697,-0.00026482,-0.00042692,-0.00059735,-0.00076875,-0.00093247,-0.0010789,-0.0011976,-0.0012781,-0.0013103,-0.0012848,-0.0011939,-0.0010319,-0.00079582,-0.0004861,-0.00010676,0.00033419,0.00082457,0.0013481,0.0018847,0.002411,0.0029007,0.0033262,0.0036588,0.0038709,0.0039364,0.0038332,0.0035436,0.0030565,0.0023682,0.0014837,0.0004172,-0.00080714,-0.0021553,-0.0035838,-0.0050402,-0.0064646,-0.0077908,-0.0089487,-0.0098667,-0.010473,-0.010701,-0.010489,-0.0097848,-0.0085466,-0.0067476,-0.0043762,-0.0014378,0.0020442,0.0060285,0.010456,0.01525,0.02032,0.025563,0.030865,0.036105,0.04116,0.045909,0.050232,0.054021,0.057179,0.059623,0.061289,0.062133,0.062133,0.061289,0.059623,0.057179,0.054021,0.050232,0.045909,0.04116,0.036105,0.030865,0.025563,0.02032,0.01525,0.010456,0.0060285,0.0020442,-0.0014378,-0.0043762,-0.0067476,-0.0085466,-0.0097848,-0.010489,-0.010701,-0.010473,-0.0098667,-0.0089487,-0.0077908,-0.0064646,-0.0050402,-0.0035838,-0.0021553,-0.00080714,0.0004172,0.0014837,0.0023682,0.0030565,0.0035436,0.0038332,0.0039364,0.0038709,0.0036588,0.0033262,0.0029007,0.002411,0.0018847,0.0013481,0.00082457,0.00033419,-0.00010676,-0.0004861,-0.00079582,-0.0010319,-0.0011939,-0.0012848,-0.0013103,-0.0012781,-0.0011976,-0.0010789,-0.00093247,-0.00076875,-0.00059735,-0.00042692,-0.00026482,-0.00011697,1.2225e-05,0.00011989,0.0002046,0.00026619,0.00030564,0.00032482,0.00032628,0.00031298,0.00028812,0.00025495,0.00021655,0.00017572,0.00013491,9.6104e-05,6.0834e-05,3.016e-05,4.699e-06,-1.5328e-05,-3.0033e-05,-3.9798e-05,-4.5194e-05,-4.692e-05,-4.5735e-05,-4.2397e-05,-3.7629e-05,-3.2061e-05,-2.6243e-05,-2.0595e-05,-1.543e-05,-1.0952e-05,-7.2608e-06,-6.3717e-06]
        self.tx_num_enabled_ch8_14 = tx_num_enabled_ch8_14 = sum([tx_ch8_enable, tx_ch9_enable, tx_ch10_enable, tx_ch11_enable, tx_ch12_enable, tx_ch13_enable, tx_ch14_enable])
        self.tx_num_enabled_ch1_7 = tx_num_enabled_ch1_7 = sum([tx_ch1_enable, tx_ch2_enable, tx_ch3_enable, tx_ch4_enable, tx_ch5_enable, tx_ch6_enable, tx_ch7_enable])
        self.tx_interp_fir2_taps = tx_interp_fir2_taps = [0.00047746,0.0021732,0.0050807,0.0064914,0.00028582,-0.019702,-0.051662,-0.077639,-0.063579,0.02661,0.20688,0.44923,0.68473,0.8306,0.8306,0.68473,0.44923,0.20688,0.02661,-0.063579,-0.077639,-0.051662,-0.019702,0.00028582,0.0064914,0.0050807,0.0021732,0.00047746]
        self.tx_interp_fir1_taps = tx_interp_fir1_taps = [-4.8644e-05,-0.00016723,-0.00034107,-0.00044056,-0.00022109,0.00056199,0.0019097,0.0033168,0.0037097,0.0018038,-0.0030821,-0.01005,-0.016097,-0.01671,-0.0078332,0.011277,0.035683,0.054566,0.054373,0.025017,-0.032996,-0.10389,-0.1576,-0.15779,-0.074999,0.09961,0.34588,0.61616,0.84749,0.98091,0.98091,0.84749,0.61616,0.34588,0.09961,-0.074999,-0.15779,-0.1576,-0.10389,-0.032996,0.025017,0.054373,0.054566,0.035683,0.011277,-0.0078332,-0.01671,-0.016097,-0.01005,-0.0030821,0.0018038,0.0037097,0.0033168,0.0019097,0.00056199,-0.00022109,-0.00044056,-0.00034107,-0.00016723,-4.8644e-05]
        self.tx_cont = tx_cont = False
        self.tx_ch_label = tx_ch_label = ""
        self.tx_ch4_offset = tx_ch4_offset = 462.6375e6 - tx_rf_freq
        self.tx_ch11_offset = tx_ch11_offset = 467.6375e6 - tx_rf_freq
        self.tx_audio_gain = tx_audio_gain = 1
        self.rx_xlating_fir_taps = rx_xlating_fir_taps = [-4.7805e-05,-0.00019404,-0.00053601,-0.0011824,-0.0022208,-0.0036578,-0.0053495,-0.0069408,-0.0078402,-0.0072531,-0.0042895,0.001863,0.011725,0.025321,0.042041,0.060612,0.079218,0.095753,0.10816,0.11482,0.11482,0.10816,0.095753,0.079218,0.060612,0.042041,0.025321,0.011725,0.001863,-0.0042895,-0.0072531,-0.0078402,-0.0069408,-0.0053495,-0.0036578,-0.0022208,-0.0011824,-0.00053601,-0.00019404,-4.7805e-05]
        self.rx_vga_gain = rx_vga_gain = 33
        self.rx_squelch = rx_squelch = -45
        self.rx_rf_sample_rate = rx_rf_sample_rate = 8e6
        self.rx_power_squelch_alpha = rx_power_squelch_alpha = 0.0125
        self.rx_pc_taps = rx_pc_taps = [-0.00045208,-4.474e-05,-4.0307e-05,-3.0817e-05,-1.6342e-05,3.4432e-06,2.8181e-05,5.7893e-05,9.1868e-05,0.00012974,0.0001704,0.00021304,0.00025609,0.00029836,0.00033796,0.00037353,0.00040315,0.00042547,0.00043837,0.00043955,0.00042895,0.00040433,0.0003649,0.00031024,0.00023988,0.00015429,5.3909e-05,-5.9804e-05,-0.00018539,-0.00032035,-0.00046214,-0.00060725,-0.00075225,-0.00089289,-0.0010252,-0.0011443,-0.0012459,-0.0013251,-0.0013782,-0.0014005,-0.001389,-0.0013403,-0.0012525,-0.0011239,-0.00095447,-0.0007445,-0.000496,-0.00021167,0.00010412,0.00044629,0.00080818,0.0011825,0.0015606,0.0019335,0.0022909,0.0026229,0.0029187,0.0031679,0.0033603,0.0034864,0.0035374,0.0035059,0.0033856,0.0031722,0.002863,0.0024579,0.0019586,0.0013694,0.00069708,-4.8959e-05,-0.00085709,-0.001713,-0.0026004,-0.0035007,-0.0043939,-0.005258,-0.0060704,-0.0068075,-0.0074456,-0.007961,-0.008331,-0.0085338,-0.0085494,-0.0083598,-0.0079499,-0.0073073,-0.0064233,-0.0052925,-0.0039139,-0.0022905,-0.00042946,0.0016577,0.0039549,0.0064425,0.0090964,0.011889,0.01479,0.017766,0.020781,0.023798,0.026777,0.029681,0.03247,0.035106,0.037553,0.039777,0.041747,0.043433,0.044813,0.045865,0.046576,0.046933,0.046933,0.046576,0.045865,0.044813,0.043433,0.041747,0.039777,0.037553,0.035106,0.03247,0.029681,0.026777,0.023798,0.020781,0.017766,0.01479,0.011889,0.0090964,0.0064425,0.0039549,0.0016577,-0.00042946,-0.0022905,-0.0039139,-0.0052925,-0.0064233,-0.0073073,-0.0079499,-0.0083598,-0.0085494,-0.0085338,-0.008331,-0.007961,-0.0074456,-0.0068075,-0.0060704,-0.005258,-0.0043939,-0.0035007,-0.0026004,-0.001713,-0.00085709,-4.8959e-05,0.00069708,0.0013694,0.0019586,0.0024579,0.002863,0.0031722,0.0033856,0.0035059,0.0035374,0.0034864,0.0033603,0.0031679,0.0029187,0.0026229,0.0022909,0.0019335,0.0015606,0.0011825,0.00080818,0.00044629,0.00010412,-0.00021167,-0.000496,-0.0007445,-0.00095447,-0.0011239,-0.0012525,-0.0013403,-0.001389,-0.0014005,-0.0013782,-0.0013251,-0.0012459,-0.0011443,-0.0010252,-0.00089289,-0.00075225,-0.00060725,-0.00046214,-0.00032035,-0.00018539,-5.9804e-05,5.3909e-05,0.00015429,0.00023988,0.00031024,0.0003649,0.00040433,0.00042895,0.00043955,0.00043837,0.00042547,0.00040315,0.00037353,0.00033796,0.00029836,0.00025609,0.00021304,0.0001704,0.00012974,9.1868e-05,5.7893e-05,2.8181e-05,3.4432e-06,-1.6342e-05,-3.0817e-05,-4.0307e-05,-4.474e-05,-0.00045208]
        self.rx_lna_gain = rx_lna_gain = 6
        self.rx_enable = rx_enable = True
        self.rx_dec_fir_taps = rx_dec_fir_taps = [-1.0662e-05,6.8043e-06,2.4381e-05,5.1631e-05,8.2064e-05,0.00010369,0.00010047,5.6519e-05,-3.757e-05,-0.00017776,-0.00034135,-0.00048602,-0.00055545,-0.00049196,-0.00025471,0.00016054,0.00070578,0.0012759,0.0017192,0.001866,0.001571,0.00076303,-0.00051222,-0.0020683,-0.0035871,-0.0046655,-0.0048987,-0.003985,-0.0018318,0.0013662,0.0051016,0.0086021,0.010954,0.011289,0.0090026,0.0039584,-0.0033701,-0.011863,-0.019851,-0.025341,-0.026348,-0.021276,-0.0092719,0.0095133,0.033821,0.061376,0.089168,0.11388,0.1324,0.14232,0.14232,0.1324,0.11388,0.089168,0.061376,0.033821,0.0095133,-0.0092719,-0.021276,-0.026348,-0.025341,-0.019851,-0.011863,-0.0033701,0.0039584,0.0090026,0.011289,0.010954,0.0086021,0.0051016,0.0013662,-0.0018318,-0.003985,-0.0048987,-0.0046655,-0.0035871,-0.0020683,-0.00051222,0.00076303,0.001571,0.001866,0.0017192,0.0012759,0.00070578,0.00016054,-0.00025471,-0.00049196,-0.00055545,-0.00048602,-0.00034135,-0.00017776,-3.757e-05,5.6519e-05,0.00010047,0.00010369,8.2064e-05,5.1631e-05,2.4381e-05,6.8043e-06,-1.0662e-05]
        self.rx_ctcss_squelch = rx_ctcss_squelch = 0.010
        self.rx_ctcss_len = rx_ctcss_len = int(0.250 * rx_audio_sample_rate)
        self.rx_ch_label = rx_ch_label = ""
        self.rx_ch9_enable = rx_ch9_enable = False
        self.rx_ch8_enable = rx_ch8_enable = True
        self.rx_ch7_enable = rx_ch7_enable = False
        self.rx_ch6_enable = rx_ch6_enable = False
        self.rx_ch5_enable = rx_ch5_enable = False
        self.rx_ch4_offset = rx_ch4_offset = 462.6375e6 - rf_rx_freq
        self.rx_ch4_enable = rx_ch4_enable = False
        self.rx_ch3_enable = rx_ch3_enable = False
        self.rx_ch2_enable = rx_ch2_enable = False
        self.rx_ch1_enable = rx_ch1_enable = False
        self.rx_ch14_enable = rx_ch14_enable = False
        self.rx_ch13_enable = rx_ch13_enable = False
        self.rx_ch12_enable = rx_ch12_enable = False
        self.rx_ch11_offset = rx_ch11_offset = 467.6375e6 - rf_rx_freq
        self.rx_ch11_enable = rx_ch11_enable = False
        self.rx_ch10_enable = rx_ch10_enable = False
        self.nbfm_tau = nbfm_tau = 5e-6
        self.ctcss_hpf_ff_taps = ctcss_hpf_ff_taps = [0.77275029037678344412398701024358160793781280517578125,-4.62934886736234840753922981093637645244598388671875,11.562660778112377357729201321490108966827392578125,-15.4121243939073497841718562995083630084991455078125,11.562660778112377357729201321490108966827392578125,-4.62934886736234840753922981093637645244598388671875,0.77275029037678344412398701024358160793781280517578125]
        self.ctcss_hpf_fb_taps = ctcss_hpf_fb_taps = [1,-5.6992016578761877099168486893177032470703125,13.56101351514487163285593851469457149505615234375,-17.24510428469104539317413582466542720794677734375,12.3620564992096202416860251105390489101409912109375,-4.7368104535795811926845999551005661487579345703125,0.7580490211161876601408948772586882114410400390625]
        self.ctcss_enable = ctcss_enable = (ctcss_freq != 0)

        ##################################################
        # Blocks
        ##################################################
        self.tabs = Qt.QTabWidget()
        self.tabs_widget_0 = Qt.QWidget()
        self.tabs_layout_0 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tabs_widget_0)
        self.tabs_grid_layout_0 = Qt.QGridLayout()
        self.tabs_layout_0.addLayout(self.tabs_grid_layout_0)
        self.tabs.addTab(self.tabs_widget_0, "RX")
        self.tabs_widget_1 = Qt.QWidget()
        self.tabs_layout_1 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tabs_widget_1)
        self.tabs_grid_layout_1 = Qt.QGridLayout()
        self.tabs_layout_1.addLayout(self.tabs_grid_layout_1)
        self.tabs.addTab(self.tabs_widget_1, "TX")
        self.top_grid_layout.addWidget(self.tabs, 1, 0, 6, 16)
        self._tx_vga2_gain_range = Range(0, 20, 1, 10, 200)
        self._tx_vga2_gain_win = RangeWidget(self._tx_vga2_gain_range, self.set_tx_vga2_gain, "VGA2 Gain", "counter_slider", float)
        self.tabs_grid_layout_1.addWidget(self._tx_vga2_gain_win,  1, 10, 1, 5)
        self._tx_vga1_gain_range = Range(-35, -4, 1, -8, 200)
        self._tx_vga1_gain_win = RangeWidget(self._tx_vga1_gain_range, self.set_tx_vga1_gain, "VGA1 Gain", "counter_slider", float)
        self.tabs_grid_layout_1.addWidget(self._tx_vga1_gain_win,  1, 5, 1, 5)
        _tx_ptt_push_button = Qt.QPushButton("Push to Talk")
        self._tx_ptt_choices = {'Pressed': True, 'Released': False}
        _tx_ptt_push_button.pressed.connect(lambda: self.set_tx_ptt(self._tx_ptt_choices['Pressed']))
        _tx_ptt_push_button.released.connect(lambda: self.set_tx_ptt(self._tx_ptt_choices['Released']))
        self.top_grid_layout.addWidget(_tx_ptt_push_button, 0, 3)
        _tx_cont_check_box = Qt.QCheckBox("Continuous TX")
        self._tx_cont_choices = {True: True, False: False}
        self._tx_cont_choices_inv = dict((v,k) for k,v in self._tx_cont_choices.iteritems())
        self._tx_cont_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_cont_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_cont_choices_inv[i]))
        self._tx_cont_callback(self.tx_cont)
        _tx_cont_check_box.stateChanged.connect(lambda i: self.set_tx_cont(self._tx_cont_choices[bool(i)]))
        self.top_grid_layout.addWidget(_tx_cont_check_box, 0, 2)
        _tx_ch9_enable_check_box = Qt.QCheckBox("9")
        self._tx_ch9_enable_choices = {True: True, False: False}
        self._tx_ch9_enable_choices_inv = dict((v,k) for k,v in self._tx_ch9_enable_choices.iteritems())
        self._tx_ch9_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch9_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch9_enable_choices_inv[i]))
        self._tx_ch9_enable_callback(self.tx_ch9_enable)
        _tx_ch9_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch9_enable(self._tx_ch9_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch9_enable_check_box,  0, 9, 1, 1)
        _tx_ch8_enable_check_box = Qt.QCheckBox("8")
        self._tx_ch8_enable_choices = {True: True, False: False}
        self._tx_ch8_enable_choices_inv = dict((v,k) for k,v in self._tx_ch8_enable_choices.iteritems())
        self._tx_ch8_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch8_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch8_enable_choices_inv[i]))
        self._tx_ch8_enable_callback(self.tx_ch8_enable)
        _tx_ch8_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch8_enable(self._tx_ch8_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch8_enable_check_box,  0, 8, 1, 1)
        _tx_ch7_enable_check_box = Qt.QCheckBox("7")
        self._tx_ch7_enable_choices = {True: True, False: False}
        self._tx_ch7_enable_choices_inv = dict((v,k) for k,v in self._tx_ch7_enable_choices.iteritems())
        self._tx_ch7_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch7_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch7_enable_choices_inv[i]))
        self._tx_ch7_enable_callback(self.tx_ch7_enable)
        _tx_ch7_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch7_enable(self._tx_ch7_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch7_enable_check_box,  0, 7, 1, 1)
        _tx_ch6_enable_check_box = Qt.QCheckBox("6")
        self._tx_ch6_enable_choices = {True: True, False: False}
        self._tx_ch6_enable_choices_inv = dict((v,k) for k,v in self._tx_ch6_enable_choices.iteritems())
        self._tx_ch6_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch6_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch6_enable_choices_inv[i]))
        self._tx_ch6_enable_callback(self.tx_ch6_enable)
        _tx_ch6_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch6_enable(self._tx_ch6_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch6_enable_check_box,  0, 6, 1, 1)
        _tx_ch5_enable_check_box = Qt.QCheckBox("5")
        self._tx_ch5_enable_choices = {True: True, False: False}
        self._tx_ch5_enable_choices_inv = dict((v,k) for k,v in self._tx_ch5_enable_choices.iteritems())
        self._tx_ch5_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch5_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch5_enable_choices_inv[i]))
        self._tx_ch5_enable_callback(self.tx_ch5_enable)
        _tx_ch5_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch5_enable(self._tx_ch5_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch5_enable_check_box,  0, 5, 1, 1)
        _tx_ch4_enable_check_box = Qt.QCheckBox("4")
        self._tx_ch4_enable_choices = {True: True, False: False}
        self._tx_ch4_enable_choices_inv = dict((v,k) for k,v in self._tx_ch4_enable_choices.iteritems())
        self._tx_ch4_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch4_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch4_enable_choices_inv[i]))
        self._tx_ch4_enable_callback(self.tx_ch4_enable)
        _tx_ch4_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch4_enable(self._tx_ch4_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch4_enable_check_box,  0, 4, 1, 1)
        _tx_ch3_enable_check_box = Qt.QCheckBox("3")
        self._tx_ch3_enable_choices = {True: True, False: False}
        self._tx_ch3_enable_choices_inv = dict((v,k) for k,v in self._tx_ch3_enable_choices.iteritems())
        self._tx_ch3_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch3_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch3_enable_choices_inv[i]))
        self._tx_ch3_enable_callback(self.tx_ch3_enable)
        _tx_ch3_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch3_enable(self._tx_ch3_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch3_enable_check_box,  0, 3, 1, 1)
        _tx_ch2_enable_check_box = Qt.QCheckBox("2")
        self._tx_ch2_enable_choices = {True: True, False: False}
        self._tx_ch2_enable_choices_inv = dict((v,k) for k,v in self._tx_ch2_enable_choices.iteritems())
        self._tx_ch2_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch2_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch2_enable_choices_inv[i]))
        self._tx_ch2_enable_callback(self.tx_ch2_enable)
        _tx_ch2_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch2_enable(self._tx_ch2_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch2_enable_check_box,  0, 2, 1, 1)
        _tx_ch1_enable_check_box = Qt.QCheckBox("1")
        self._tx_ch1_enable_choices = {True: True, False: False}
        self._tx_ch1_enable_choices_inv = dict((v,k) for k,v in self._tx_ch1_enable_choices.iteritems())
        self._tx_ch1_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch1_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch1_enable_choices_inv[i]))
        self._tx_ch1_enable_callback(self.tx_ch1_enable)
        _tx_ch1_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch1_enable(self._tx_ch1_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch1_enable_check_box,  0, 1, 1, 1)
        _tx_ch14_enable_check_box = Qt.QCheckBox("14")
        self._tx_ch14_enable_choices = {True: True, False: False}
        self._tx_ch14_enable_choices_inv = dict((v,k) for k,v in self._tx_ch14_enable_choices.iteritems())
        self._tx_ch14_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch14_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch14_enable_choices_inv[i]))
        self._tx_ch14_enable_callback(self.tx_ch14_enable)
        _tx_ch14_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch14_enable(self._tx_ch14_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch14_enable_check_box,  0, 14, 1, 1)
        _tx_ch13_enable_check_box = Qt.QCheckBox("13")
        self._tx_ch13_enable_choices = {True: True, False: False}
        self._tx_ch13_enable_choices_inv = dict((v,k) for k,v in self._tx_ch13_enable_choices.iteritems())
        self._tx_ch13_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch13_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch13_enable_choices_inv[i]))
        self._tx_ch13_enable_callback(self.tx_ch13_enable)
        _tx_ch13_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch13_enable(self._tx_ch13_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch13_enable_check_box,  0, 13, 1, 1)
        _tx_ch12_enable_check_box = Qt.QCheckBox("12")
        self._tx_ch12_enable_choices = {True: True, False: False}
        self._tx_ch12_enable_choices_inv = dict((v,k) for k,v in self._tx_ch12_enable_choices.iteritems())
        self._tx_ch12_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch12_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch12_enable_choices_inv[i]))
        self._tx_ch12_enable_callback(self.tx_ch12_enable)
        _tx_ch12_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch12_enable(self._tx_ch12_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch12_enable_check_box,  0, 12, 1, 1)
        _tx_ch11_enable_check_box = Qt.QCheckBox("11")
        self._tx_ch11_enable_choices = {True: True, False: False}
        self._tx_ch11_enable_choices_inv = dict((v,k) for k,v in self._tx_ch11_enable_choices.iteritems())
        self._tx_ch11_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch11_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch11_enable_choices_inv[i]))
        self._tx_ch11_enable_callback(self.tx_ch11_enable)
        _tx_ch11_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch11_enable(self._tx_ch11_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch11_enable_check_box,  0, 11, 1, 1)
        _tx_ch10_enable_check_box = Qt.QCheckBox("10")
        self._tx_ch10_enable_choices = {True: True, False: False}
        self._tx_ch10_enable_choices_inv = dict((v,k) for k,v in self._tx_ch10_enable_choices.iteritems())
        self._tx_ch10_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_tx_ch10_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._tx_ch10_enable_choices_inv[i]))
        self._tx_ch10_enable_callback(self.tx_ch10_enable)
        _tx_ch10_enable_check_box.stateChanged.connect(lambda i: self.set_tx_ch10_enable(self._tx_ch10_enable_choices[bool(i)]))
        self.tabs_grid_layout_1.addWidget(_tx_ch10_enable_check_box,  0, 10, 1, 1)
        self._tx_audio_gain_range = Range(0, 5, 0.0125, 1, 200)
        self._tx_audio_gain_win = RangeWidget(self._tx_audio_gain_range, self.set_tx_audio_gain, "Audio Gain", "counter_slider", float)
        self.tabs_grid_layout_1.addWidget(self._tx_audio_gain_win,  1, 0, 1, 5)
        self._rx_vga_gain_range = Range(5, 60, 1, 33, 200)
        self._rx_vga_gain_win = RangeWidget(self._rx_vga_gain_range, self.set_rx_vga_gain, "RX VGA1+VGA2", "counter_slider", float)
        self.tabs_grid_layout_0.addWidget(self._rx_vga_gain_win,  1, 1, 1, 4)
        self._rx_squelch_range = Range(-100, 0, 1, -45, 200)
        self._rx_squelch_win = RangeWidget(self._rx_squelch_range, self.set_rx_squelch, "Power Squelch (dB)", "counter_slider", float)
        self.tabs_grid_layout_0.addWidget(self._rx_squelch_win,  1, 5, 1, 4)
        self._rx_lna_gain_options = (0, 3, 6, )
        self._rx_lna_gain_labels = ("0 dB", "3 dB", "6  dB", )
        self._rx_lna_gain_tool_bar = Qt.QToolBar(self)
        self._rx_lna_gain_tool_bar.addWidget(Qt.QLabel("LNA Gain"+": "))
        self._rx_lna_gain_combo_box = Qt.QComboBox()
        self._rx_lna_gain_tool_bar.addWidget(self._rx_lna_gain_combo_box)
        for label in self._rx_lna_gain_labels: self._rx_lna_gain_combo_box.addItem(label)
        self._rx_lna_gain_callback = lambda i: Qt.QMetaObject.invokeMethod(self._rx_lna_gain_combo_box, "setCurrentIndex", Qt.Q_ARG("int", self._rx_lna_gain_options.index(i)))
        self._rx_lna_gain_callback(self.rx_lna_gain)
        self._rx_lna_gain_combo_box.currentIndexChanged.connect(
        	lambda i: self.set_rx_lna_gain(self._rx_lna_gain_options[i]))
        self.tabs_grid_layout_0.addWidget(self._rx_lna_gain_tool_bar,  1, 0, 1, 1)
        _rx_enable_check_box = Qt.QCheckBox("Enable RX")
        self._rx_enable_choices = {True: True, False: False}
        self._rx_enable_choices_inv = dict((v,k) for k,v in self._rx_enable_choices.iteritems())
        self._rx_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_enable_choices_inv[i]))
        self._rx_enable_callback(self.rx_enable)
        _rx_enable_check_box.stateChanged.connect(lambda i: self.set_rx_enable(self._rx_enable_choices[bool(i)]))
        self.top_grid_layout.addWidget(_rx_enable_check_box, 0, 1)
        _rx_ch9_enable_check_box = Qt.QCheckBox("9")
        self._rx_ch9_enable_choices = {True: True, False: False}
        self._rx_ch9_enable_choices_inv = dict((v,k) for k,v in self._rx_ch9_enable_choices.iteritems())
        self._rx_ch9_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch9_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch9_enable_choices_inv[i]))
        self._rx_ch9_enable_callback(self.rx_ch9_enable)
        _rx_ch9_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch9_enable(self._rx_ch9_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch9_enable_check_box,  0, 9, 1, 1)
        _rx_ch8_enable_check_box = Qt.QCheckBox("8")
        self._rx_ch8_enable_choices = {True: True, False: False}
        self._rx_ch8_enable_choices_inv = dict((v,k) for k,v in self._rx_ch8_enable_choices.iteritems())
        self._rx_ch8_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch8_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch8_enable_choices_inv[i]))
        self._rx_ch8_enable_callback(self.rx_ch8_enable)
        _rx_ch8_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch8_enable(self._rx_ch8_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch8_enable_check_box,  0, 8, 1, 1)
        _rx_ch7_enable_check_box = Qt.QCheckBox("7")
        self._rx_ch7_enable_choices = {True: True, False: False}
        self._rx_ch7_enable_choices_inv = dict((v,k) for k,v in self._rx_ch7_enable_choices.iteritems())
        self._rx_ch7_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch7_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch7_enable_choices_inv[i]))
        self._rx_ch7_enable_callback(self.rx_ch7_enable)
        _rx_ch7_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch7_enable(self._rx_ch7_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch7_enable_check_box,  0, 7, 1, 1)
        _rx_ch6_enable_check_box = Qt.QCheckBox("6")
        self._rx_ch6_enable_choices = {True: True, False: False}
        self._rx_ch6_enable_choices_inv = dict((v,k) for k,v in self._rx_ch6_enable_choices.iteritems())
        self._rx_ch6_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch6_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch6_enable_choices_inv[i]))
        self._rx_ch6_enable_callback(self.rx_ch6_enable)
        _rx_ch6_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch6_enable(self._rx_ch6_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch6_enable_check_box,  0, 6, 1, 1)
        _rx_ch5_enable_check_box = Qt.QCheckBox("5")
        self._rx_ch5_enable_choices = {True: True, False: False}
        self._rx_ch5_enable_choices_inv = dict((v,k) for k,v in self._rx_ch5_enable_choices.iteritems())
        self._rx_ch5_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch5_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch5_enable_choices_inv[i]))
        self._rx_ch5_enable_callback(self.rx_ch5_enable)
        _rx_ch5_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch5_enable(self._rx_ch5_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch5_enable_check_box,  0, 5, 1, 1)
        _rx_ch4_enable_check_box = Qt.QCheckBox("4")
        self._rx_ch4_enable_choices = {True: True, False: False}
        self._rx_ch4_enable_choices_inv = dict((v,k) for k,v in self._rx_ch4_enable_choices.iteritems())
        self._rx_ch4_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch4_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch4_enable_choices_inv[i]))
        self._rx_ch4_enable_callback(self.rx_ch4_enable)
        _rx_ch4_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch4_enable(self._rx_ch4_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch4_enable_check_box,  0, 4, 1, 1)
        _rx_ch3_enable_check_box = Qt.QCheckBox("3")
        self._rx_ch3_enable_choices = {True: True, False: False}
        self._rx_ch3_enable_choices_inv = dict((v,k) for k,v in self._rx_ch3_enable_choices.iteritems())
        self._rx_ch3_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch3_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch3_enable_choices_inv[i]))
        self._rx_ch3_enable_callback(self.rx_ch3_enable)
        _rx_ch3_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch3_enable(self._rx_ch3_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch3_enable_check_box,  0, 3, 1, 1)
        _rx_ch2_enable_check_box = Qt.QCheckBox("2")
        self._rx_ch2_enable_choices = {True: True, False: False}
        self._rx_ch2_enable_choices_inv = dict((v,k) for k,v in self._rx_ch2_enable_choices.iteritems())
        self._rx_ch2_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch2_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch2_enable_choices_inv[i]))
        self._rx_ch2_enable_callback(self.rx_ch2_enable)
        _rx_ch2_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch2_enable(self._rx_ch2_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch2_enable_check_box,  0, 2, 1, 1)
        _rx_ch1_enable_check_box = Qt.QCheckBox("1")
        self._rx_ch1_enable_choices = {True: True, False: False}
        self._rx_ch1_enable_choices_inv = dict((v,k) for k,v in self._rx_ch1_enable_choices.iteritems())
        self._rx_ch1_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch1_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch1_enable_choices_inv[i]))
        self._rx_ch1_enable_callback(self.rx_ch1_enable)
        _rx_ch1_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch1_enable(self._rx_ch1_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch1_enable_check_box,  0, 1, 1, 1)
        _rx_ch14_enable_check_box = Qt.QCheckBox("14")
        self._rx_ch14_enable_choices = {True: True, False: False}
        self._rx_ch14_enable_choices_inv = dict((v,k) for k,v in self._rx_ch14_enable_choices.iteritems())
        self._rx_ch14_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch14_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch14_enable_choices_inv[i]))
        self._rx_ch14_enable_callback(self.rx_ch14_enable)
        _rx_ch14_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch14_enable(self._rx_ch14_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch14_enable_check_box,  0, 14, 1, 1)
        _rx_ch13_enable_check_box = Qt.QCheckBox("13")
        self._rx_ch13_enable_choices = {True: True, False: False}
        self._rx_ch13_enable_choices_inv = dict((v,k) for k,v in self._rx_ch13_enable_choices.iteritems())
        self._rx_ch13_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch13_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch13_enable_choices_inv[i]))
        self._rx_ch13_enable_callback(self.rx_ch13_enable)
        _rx_ch13_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch13_enable(self._rx_ch13_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch13_enable_check_box,  0, 13, 1, 1)
        _rx_ch12_enable_check_box = Qt.QCheckBox("12")
        self._rx_ch12_enable_choices = {True: True, False: False}
        self._rx_ch12_enable_choices_inv = dict((v,k) for k,v in self._rx_ch12_enable_choices.iteritems())
        self._rx_ch12_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch12_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch12_enable_choices_inv[i]))
        self._rx_ch12_enable_callback(self.rx_ch12_enable)
        _rx_ch12_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch12_enable(self._rx_ch12_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch12_enable_check_box,  0, 12, 1, 1)
        _rx_ch11_enable_check_box = Qt.QCheckBox("11")
        self._rx_ch11_enable_choices = {True: True, False: False}
        self._rx_ch11_enable_choices_inv = dict((v,k) for k,v in self._rx_ch11_enable_choices.iteritems())
        self._rx_ch11_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch11_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch11_enable_choices_inv[i]))
        self._rx_ch11_enable_callback(self.rx_ch11_enable)
        _rx_ch11_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch11_enable(self._rx_ch11_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch11_enable_check_box,  0, 11, 1, 1)
        _rx_ch10_enable_check_box = Qt.QCheckBox("10")
        self._rx_ch10_enable_choices = {True: True, False: False}
        self._rx_ch10_enable_choices_inv = dict((v,k) for k,v in self._rx_ch10_enable_choices.iteritems())
        self._rx_ch10_enable_callback = lambda i: Qt.QMetaObject.invokeMethod(_rx_ch10_enable_check_box, "setChecked", Qt.Q_ARG("bool", self._rx_ch10_enable_choices_inv[i]))
        self._rx_ch10_enable_callback(self.rx_ch10_enable)
        _rx_ch10_enable_check_box.stateChanged.connect(lambda i: self.set_rx_ch10_enable(self._rx_ch10_enable_choices[bool(i)]))
        self.tabs_grid_layout_0.addWidget(_rx_ch10_enable_check_box,  0, 10, 1, 1)
        self._ctcss_freq_options = [0, 67.0,  71.9,  74.4,  77.0,  79.7,  82.5,  85.4,  88.5,  91.5,  94.8, 97.4, 100.0, 103.5, 107.2, 110.9, 114.8, 118.8, 123.0, 127.3, 131.8, 136.5, 141.3, 146.2, 151.4, 156.7, 162.2, 167.9, 173.8, 179.9, 186.2, 192.8, 203.5, 210.7, 218.1, 225.7, 233.6, 241.8, 250.3]
        self._ctcss_freq_labels = ["None", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38"]
        self._ctcss_freq_tool_bar = Qt.QToolBar(self)
        self._ctcss_freq_tool_bar.addWidget(Qt.QLabel("CTCSS"+": "))
        self._ctcss_freq_combo_box = Qt.QComboBox()
        self._ctcss_freq_tool_bar.addWidget(self._ctcss_freq_combo_box)
        for label in self._ctcss_freq_labels: self._ctcss_freq_combo_box.addItem(label)
        self._ctcss_freq_callback = lambda i: Qt.QMetaObject.invokeMethod(self._ctcss_freq_combo_box, "setCurrentIndex", Qt.Q_ARG("int", self._ctcss_freq_options.index(i)))
        self._ctcss_freq_callback(self.ctcss_freq)
        self._ctcss_freq_combo_box.currentIndexChanged.connect(
        	lambda i: self.set_ctcss_freq(self._ctcss_freq_options[i]))
        self.top_grid_layout.addWidget(self._ctcss_freq_tool_bar, 0, 0)
        self.tx_valve = grc_blks2.valve(item_size=gr.sizeof_float*1, open=bool(not (tx_ptt or tx_cont)))
        self.tx_post_ps_gain_ch8_14 = blocks.multiply_const_vcc((1 if tx_num_enabled_ch8_14 == 0 else 16 * 0.95 / tx_num_enabled_ch8_14, ))
        self.tx_post_ps_gain_ch1_7 = blocks.multiply_const_vcc((1 if tx_num_enabled_ch1_7 == 0 else 16 * 0.95 / tx_num_enabled_ch1_7, ))
        self.tx_nbfm = analog.nbfm_tx(
        	audio_rate=tx_audio_sample_rate,
        	quad_rate=tx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
                )
        self.tx_ctcss_tone = analog.sig_source_f(tx_audio_sample_rate, analog.GR_COS_WAVE, ctcss_freq, 0.375, 0)
        self.tx_ctcss_add = blocks.add_vff(1)
        self._tx_ch_label_tool_bar = Qt.QToolBar(self)
        
        if None:
          self._tx_ch_label_formatter = None
        else:
          self._tx_ch_label_formatter = lambda x: x
        
        self._tx_ch_label_tool_bar.addWidget(Qt.QLabel("Channel"+": "))
        self._tx_ch_label_label = Qt.QLabel(str(self._tx_ch_label_formatter(self.tx_ch_label)))
        self._tx_ch_label_tool_bar.addWidget(self._tx_ch_label_label)
        self.tabs_grid_layout_1.addWidget(self._tx_ch_label_tool_bar,  0, 0, 1, 1)
          
        self.tx_ch9_mute = blocks.mute_cc(bool(not tx_ch9_enable))
        self.tx_ch8_mute = blocks.mute_cc(bool(not tx_ch8_enable))
        self.tx_ch8_14_waterfall = qtgui.waterfall_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	tx_rf_freq + tx_ch11_offset, #fc
        	tx_pfs_output_rate, #bw
        	"Channels 8-14", #name
                1 #number of inputs
        )
        self.tx_ch8_14_waterfall.set_update_time(0.10)
        self.tx_ch8_14_waterfall.enable_grid(False)
        
        if not True:
          self.tx_ch8_14_waterfall.disable_legend()
        
        if "complex" == "float" or "complex" == "msg_float":
          self.tx_ch8_14_waterfall.set_plot_pos_half(not True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.tx_ch8_14_waterfall.set_line_label(i, "Data {0}".format(i))
            else:
                self.tx_ch8_14_waterfall.set_line_label(i, labels[i])
            self.tx_ch8_14_waterfall.set_color_map(i, colors[i])
            self.tx_ch8_14_waterfall.set_line_alpha(i, alphas[i])
        
        self.tx_ch8_14_waterfall.set_intensity_range(-140, 10)
        
        self._tx_ch8_14_waterfall_win = sip.wrapinstance(self.tx_ch8_14_waterfall.pyqwidget(), Qt.QWidget)
        self.tabs_grid_layout_1.addWidget(self._tx_ch8_14_waterfall_win, 3,9,4,8)
        self.tx_ch8_14_pfs = filter.pfb_synthesizer_ccf(
        	  16, (tx_pfs_taps), False)
        self.tx_ch8_14_pfs.set_channel_map(([13, 14, 15, 0, 1, 2, 3]))
        self.tx_ch8_14_pfs.declare_sample_delay(0)
        	
        self.tx_ch8_14_fir2 = filter.interp_fir_filter_ccc(4, (tx_interp_fir2_taps))
        self.tx_ch8_14_fir2.declare_sample_delay(0)
        self.tx_ch8_14_fir1 = filter.interp_fir_filter_ccc(5, (tx_interp_fir1_taps))
        self.tx_ch8_14_fir1.declare_sample_delay(0)
        self.tx_ch7_mute = blocks.mute_cc(bool(not tx_ch7_enable))
        self.tx_ch6_mute = blocks.mute_cc(bool(not tx_ch6_enable))
        self.tx_ch5_mute = blocks.mute_cc(bool(not tx_ch5_enable))
        self.tx_ch4_mute = blocks.mute_cc(bool(not tx_ch4_enable))
        self.tx_ch3_mute = blocks.mute_cc(bool(not tx_ch3_enable))
        self.tx_ch2_mute = blocks.mute_cc(bool(not tx_ch2_enable))
        self.tx_ch1_mute = blocks.mute_cc(bool(not tx_ch1_enable))
        self.tx_ch1_7_waterfall = qtgui.waterfall_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	tx_rf_freq + tx_ch4_offset, #fc
        	tx_pfs_output_rate, #bw
        	"Channels 1-7", #name
                1 #number of inputs
        )
        self.tx_ch1_7_waterfall.set_update_time(0.10)
        self.tx_ch1_7_waterfall.enable_grid(False)
        
        if not True:
          self.tx_ch1_7_waterfall.disable_legend()
        
        if "complex" == "float" or "complex" == "msg_float":
          self.tx_ch1_7_waterfall.set_plot_pos_half(not True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.tx_ch1_7_waterfall.set_line_label(i, "Data {0}".format(i))
            else:
                self.tx_ch1_7_waterfall.set_line_label(i, labels[i])
            self.tx_ch1_7_waterfall.set_color_map(i, colors[i])
            self.tx_ch1_7_waterfall.set_line_alpha(i, alphas[i])
        
        self.tx_ch1_7_waterfall.set_intensity_range(-140, 10)
        
        self._tx_ch1_7_waterfall_win = sip.wrapinstance(self.tx_ch1_7_waterfall.pyqwidget(), Qt.QWidget)
        self.tabs_grid_layout_1.addWidget(self._tx_ch1_7_waterfall_win, 3,0,4,8)
        self.tx_ch1_7_pfs = filter.pfb_synthesizer_ccf(
        	  16, (tx_pfs_taps), False)
        self.tx_ch1_7_pfs.set_channel_map(([13, 14, 15, 0, 1, 2, 3]))
        self.tx_ch1_7_pfs.declare_sample_delay(0)
        	
        self.tx_ch1_7_fir2 = filter.interp_fir_filter_ccc(4, (tx_interp_fir2_taps))
        self.tx_ch1_7_fir2.declare_sample_delay(0)
        self.tx_ch1_7_fir1 = filter.interp_fir_filter_ccc(5, (tx_interp_fir1_taps))
        self.tx_ch1_7_fir1.declare_sample_delay(0)
        self.tx_ch14_mute = blocks.mute_cc(bool(not tx_ch14_enable))
        self.tx_ch13_mute = blocks.mute_cc(bool(not tx_ch13_enable))
        self.tx_ch12_mute = blocks.mute_cc(bool(not tx_ch12_enable))
        self.tx_ch11_mute = blocks.mute_cc(bool(not tx_ch11_enable))
        self.tx_ch10_mute = blocks.mute_cc(bool(not tx_ch10_enable))
        self.tx_audio_src_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=1 if ctcss_enable else 0,
        	output_index=0,
        )
        self.tx_audio_gain_mult2 = blocks.multiply_const_vff((0.625, ))
        self.tx_audio_gain_mult = blocks.multiply_const_vff((tx_audio_gain, ))
        self.tx_audio_fir = filter.interp_fir_filter_fff(1, ([-0.00069908,-0.00055931,-0.00038956,0.00011415,0.00082514,0.0014371,0.0015678,0.00094654,-0.00039382,-0.0020255,-0.003217,-0.0032166,-0.0016345,0.001246,0.0044105,0.0064075,0.0059408,0.0025436,-0.002973,-0.008573,-0.011647,-0.010094,-0.0034268,0.006599,0.01623,0.020917,0.017149,0.0042609,-0.014542,-0.032524,-0.041201,-0.033136,-0.0047887,0.041595,0.097875,0.15189,0.19083,0.20501,0.19083,0.15189,0.097875,0.041595,-0.0047887,-0.033136,-0.041201,-0.032524,-0.014542,0.0042609,0.017149,0.020917,0.01623,0.006599,-0.0034268,-0.010094,-0.011647,-0.008573,-0.002973,0.0025436,0.0059408,0.0064075,0.0044105,0.001246,-0.0016345,-0.0032166,-0.003217,-0.0020255,-0.00039382,0.00094654,0.0015678,0.0014371,0.00082514,0.00011415,-0.00038956,-0.00055931,-0.00069908]))
        self.tx_audio_fir.declare_sample_delay(0)
        self.single_pole_iir_filter_xx_0 = filter.single_pole_iir_filter_ff(0.05, 1)
        self.rx_enable_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_enable))
        self._rx_ch_label_tool_bar = Qt.QToolBar(self)
        
        if None:
          self._rx_ch_label_formatter = None
        else:
          self._rx_ch_label_formatter = lambda x: x
        
        self._rx_ch_label_tool_bar.addWidget(Qt.QLabel("Channel"+": "))
        self._rx_ch_label_label = Qt.QLabel(str(self._rx_ch_label_formatter(self.rx_ch_label)))
        self._rx_ch_label_tool_bar.addWidget(self._rx_ch_label_label)
        self.tabs_grid_layout_0.addWidget(self._rx_ch_label_tool_bar,  0, 0, 1, 1)
          
        self.rx_ch9_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch9_enable))
        self.rx_ch9_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch9_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch9_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch9_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch9_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch8_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch8_enable))
        self.rx_ch8_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch8_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch8_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch8_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch8_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch7_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch7_enable))
        self.rx_ch7_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch7_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch7_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch7_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch7_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch6_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch6_enable))
        self.rx_ch6_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch6_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch6_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch6_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch6_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch5_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch5_enable))
        self.rx_ch5_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch5_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch5_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch5_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch5_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch4_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch4_enable))
        self.rx_ch4_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch4_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch4_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch4_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch4_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch3_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch3_enable))
        self.rx_ch3_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch3_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch3_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch3_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch3_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch2_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch2_enable))
        self.rx_ch2_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch2_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch2_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch2_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch2_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch1_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch1_enable))
        self.rx_ch1_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch1_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch1_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch1_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch1_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch14_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch14_enable))
        self.rx_ch14_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch14_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch14_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch14_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch14_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch13_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch13_enable))
        self.rx_ch13_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch13_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch13_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch13_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch13_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch12_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch12_enable))
        self.rx_ch12_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch12_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch12_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch12_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch12_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch11_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch11_enable))
        self.rx_ch11_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch11_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch11_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch11_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch11_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.rx_ch10_valve = grc_blks2.valve(item_size=gr.sizeof_gr_complex*1, open=bool(not rx_ch10_enable))
        self.rx_ch10_nbfm = analog.nbfm_rx(
        	audio_rate=rx_audio_sample_rate,
        	quad_rate=rx_audio_sample_rate,
        	tau=nbfm_tau,
        	max_dev=2.5e3,
          )
        self.rx_ch10_hpf_iir = filter.iir_filter_ffd((ctcss_hpf_ff_taps), (ctcss_hpf_fb_taps), False)
        self.rx_ch10_audio_sink = audio.sink(rx_audio_sample_rate, "pulse", True)
        self.rx_ch10_analog_pwr_squelch = analog.pwr_squelch_cc(rx_squelch, rx_power_squelch_alpha, 0, True)
        self.rx_ch10_analog_ctcss_squelch_ff = analog.ctcss_squelch_ff(rx_audio_sample_rate, ctcss_freq, rx_ctcss_squelch, rx_ctcss_len, 0, False)
        self.qtgui_waterfall_sink_x_0_0 = qtgui.waterfall_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	rf_rx_freq + rx_ch11_offset, #fc
        	rx_rf_sample_rate / 20, #bw
        	"Channels 8-14", #name
                1 #number of inputs
        )
        self.qtgui_waterfall_sink_x_0_0.set_update_time(0.10)
        self.qtgui_waterfall_sink_x_0_0.enable_grid(False)
        
        if not True:
          self.qtgui_waterfall_sink_x_0_0.disable_legend()
        
        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_waterfall_sink_x_0_0.set_plot_pos_half(not True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_waterfall_sink_x_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_waterfall_sink_x_0_0.set_line_label(i, labels[i])
            self.qtgui_waterfall_sink_x_0_0.set_color_map(i, colors[i])
            self.qtgui_waterfall_sink_x_0_0.set_line_alpha(i, alphas[i])
        
        self.qtgui_waterfall_sink_x_0_0.set_intensity_range(-140, 10)
        
        self._qtgui_waterfall_sink_x_0_0_win = sip.wrapinstance(self.qtgui_waterfall_sink_x_0_0.pyqwidget(), Qt.QWidget)
        self.tabs_grid_layout_0.addWidget(self._qtgui_waterfall_sink_x_0_0_win,  3, 9, 2, 8)
        self.qtgui_waterfall_sink_x_0 = qtgui.waterfall_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	rf_rx_freq + rx_ch4_offset, #fc
        	rx_rf_sample_rate / 20, #bw
        	"Channels 1-7", #name
                1 #number of inputs
        )
        self.qtgui_waterfall_sink_x_0.set_update_time(0.10)
        self.qtgui_waterfall_sink_x_0.enable_grid(False)
        
        if not True:
          self.qtgui_waterfall_sink_x_0.disable_legend()
        
        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_waterfall_sink_x_0.set_plot_pos_half(not True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_waterfall_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_waterfall_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_waterfall_sink_x_0.set_color_map(i, colors[i])
            self.qtgui_waterfall_sink_x_0.set_line_alpha(i, alphas[i])
        
        self.qtgui_waterfall_sink_x_0.set_intensity_range(-140, 10)
        
        self._qtgui_waterfall_sink_x_0_win = sip.wrapinstance(self.qtgui_waterfall_sink_x_0.pyqwidget(), Qt.QWidget)
        self.tabs_grid_layout_0.addWidget(self._qtgui_waterfall_sink_x_0_win,  3, 0, 2, 8)
        self.qtgui_number_sink_0 = qtgui.number_sink(
            gr.sizeof_float,
            0,
            qtgui.NUM_GRAPH_VERT,
            1
        )
        self.qtgui_number_sink_0.set_update_time(1 / 30.0)
        self.qtgui_number_sink_0.set_title("")
        
        labels = ["     Input Power     ", "", "", "", "",
                  "", "", "", "", ""]
        units = ["dBFS", "", "", "", "",
                 "", "", "", "", ""]
        colors = [("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"),
                  ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black")]
        factor = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        for i in xrange(1):
            self.qtgui_number_sink_0.set_min(i, -50)
            self.qtgui_number_sink_0.set_max(i, 3)
            self.qtgui_number_sink_0.set_color(i, colors[i][0], colors[i][1])
            if len(labels[i]) == 0:
                self.qtgui_number_sink_0.set_label(i, "Data {0}".format(i))
            else:
                self.qtgui_number_sink_0.set_label(i, labels[i])
            self.qtgui_number_sink_0.set_unit(i, units[i])
            self.qtgui_number_sink_0.set_factor(i, factor[i])
        
        self.qtgui_number_sink_0.enable_autoscale(False)
        self._qtgui_number_sink_0_win = sip.wrapinstance(self.qtgui_number_sink_0.pyqwidget(), Qt.QWidget)
        self.tabs_grid_layout_0.addWidget(self._qtgui_number_sink_0_win, 3,8,2,1)
        self.pfb_channelizer_ch8_ch14 = pfb.channelizer_ccf(
        	  16,
        	  (rx_pc_taps),
        	  1.0,
        	  100)
        self.pfb_channelizer_ch8_ch14.set_channel_map(([13, 14, 15, 0, 1, 2, 3]))
        self.pfb_channelizer_ch8_ch14.declare_sample_delay(0)
        	
        self.pfb_channelizer_ch1_ch17 = pfb.channelizer_ccf(
        	  16,
        	  (rx_pc_taps),
        	  1.0,
        	  100)
        self.pfb_channelizer_ch1_ch17.set_channel_map(([13, 14, 15, 0, 1, 2, 3]))
        self.pfb_channelizer_ch1_ch17.declare_sample_delay(0)
        	
        self.osmosdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + "bladerf=0,buffers=128,buflen=16384,transfers=32" )
        self.osmosdr_source_0.set_sample_rate(rx_rf_sample_rate)
        self.osmosdr_source_0.set_center_freq(rf_rx_freq, 0)
        self.osmosdr_source_0.set_freq_corr(0, 0)
        self.osmosdr_source_0.set_dc_offset_mode(1, 0)
        self.osmosdr_source_0.set_iq_balance_mode(0, 0)
        self.osmosdr_source_0.set_gain_mode(False, 0)
        self.osmosdr_source_0.set_gain(rx_lna_gain, 0)
        self.osmosdr_source_0.set_if_gain(0, 0)
        self.osmosdr_source_0.set_bb_gain(rx_vga_gain, 0)
        self.osmosdr_source_0.set_antenna("", 0)
        self.osmosdr_source_0.set_bandwidth(6e6, 0)
          
        self.osmosdr_sink_0 = osmosdr.sink( args="numchan=" + str(1) + " " + "bladerf=0,buffers=128,buflen=8192,transfers=32" )
        self.osmosdr_sink_0.set_sample_rate(tx_rf_sample_rate)
        self.osmosdr_sink_0.set_center_freq(tx_rf_freq, 0)
        self.osmosdr_sink_0.set_freq_corr(0, 0)
        self.osmosdr_sink_0.set_gain(tx_vga2_gain, 0)
        self.osmosdr_sink_0.set_if_gain(0, 0)
        self.osmosdr_sink_0.set_bb_gain(tx_vga1_gain, 0)
        self.osmosdr_sink_0.set_antenna("", 0)
        self.osmosdr_sink_0.set_bandwidth(6e6, 0)
          
        self.freq_xlating_fir_filter_xxx_0_0 = filter.freq_xlating_fir_filter_ccc(4, (rx_xlating_fir_taps), 2.3e6, rx_rf_sample_rate)
        self.freq_xlating_fir_filter_xxx_0 = filter.freq_xlating_fir_filter_ccc(4, (rx_xlating_fir_taps), rx_ch4_offset, rx_rf_sample_rate)
        self.fir_filter_xxx_0_0 = filter.fir_filter_ccc(5, (rx_dec_fir_taps))
        self.fir_filter_xxx_0_0.declare_sample_delay(0)
        self.fir_filter_xxx_0 = filter.fir_filter_ccc(5, (rx_dec_fir_taps))
        self.fir_filter_xxx_0.declare_sample_delay(0)
        self.ch9_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch8_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch7_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch6_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch5_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch4_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch3_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch2_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch1_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch14_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch13_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch12_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch11_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.ch10_audio_sel = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=0 if ctcss_enable else 1,
        	output_index=0,
        )
        self.blocks_null_sink_0_1_0_0 = blocks.null_sink(gr.sizeof_gr_complex*1)
        self.blocks_null_sink_0_1_0 = blocks.null_sink(gr.sizeof_gr_complex*1)
        self.blocks_nlog10_ff_0 = blocks.nlog10_ff(10, 1, 0)
        self.blocks_multiply_xx_0_0 = blocks.multiply_vcc(1)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        self.blocks_keep_one_in_n_0 = blocks.keep_one_in_n(gr.sizeof_float*1, int(rx_rf_sample_rate / 30))
        self.blocks_complex_to_mag_squared_0 = blocks.complex_to_mag_squared(1)
        self.blocks_add_xx_1 = blocks.add_vcc(1)
        self.audio_source_0 = audio.source(tx_audio_sample_rate, "pulse", True)
        self.analog_sig_source_x_0_0 = analog.sig_source_c(tx_rf_sample_rate, analog.GR_COS_WAVE, tx_ch11_offset, 0.5, 0)
        self.analog_sig_source_x_0 = analog.sig_source_c(tx_rf_sample_rate, analog.GR_COS_WAVE, tx_ch4_offset, 0.5, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_multiply_xx_0, 0))    
        self.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_multiply_xx_0_0, 0))    
        self.connect((self.audio_source_0, 0), (self.tx_valve, 0))    
        self.connect((self.blocks_add_xx_1, 0), (self.osmosdr_sink_0, 0))    
        self.connect((self.blocks_complex_to_mag_squared_0, 0), (self.single_pole_iir_filter_xx_0, 0))    
        self.connect((self.blocks_keep_one_in_n_0, 0), (self.blocks_nlog10_ff_0, 0))    
        self.connect((self.blocks_multiply_xx_0, 0), (self.blocks_add_xx_1, 0))    
        self.connect((self.blocks_multiply_xx_0_0, 0), (self.blocks_add_xx_1, 1))    
        self.connect((self.blocks_nlog10_ff_0, 0), (self.qtgui_number_sink_0, 0))    
        self.connect((self.ch10_audio_sel, 0), (self.rx_ch10_hpf_iir, 0))    
        self.connect((self.ch11_audio_sel, 0), (self.rx_ch11_hpf_iir, 0))    
        self.connect((self.ch12_audio_sel, 0), (self.rx_ch12_hpf_iir, 0))    
        self.connect((self.ch13_audio_sel, 0), (self.rx_ch13_hpf_iir, 0))    
        self.connect((self.ch14_audio_sel, 0), (self.rx_ch14_hpf_iir, 0))    
        self.connect((self.ch1_audio_sel, 0), (self.rx_ch1_hpf_iir, 0))    
        self.connect((self.ch2_audio_sel, 0), (self.rx_ch2_hpf_iir, 0))    
        self.connect((self.ch3_audio_sel, 0), (self.rx_ch3_hpf_iir, 0))    
        self.connect((self.ch4_audio_sel, 0), (self.rx_ch4_hpf_iir, 0))    
        self.connect((self.ch5_audio_sel, 0), (self.rx_ch5_hpf_iir, 0))    
        self.connect((self.ch6_audio_sel, 0), (self.rx_ch6_hpf_iir, 0))    
        self.connect((self.ch7_audio_sel, 0), (self.rx_ch7_hpf_iir, 0))    
        self.connect((self.ch8_audio_sel, 0), (self.rx_ch8_hpf_iir, 0))    
        self.connect((self.ch9_audio_sel, 0), (self.rx_ch9_hpf_iir, 0))    
        self.connect((self.fir_filter_xxx_0, 0), (self.pfb_channelizer_ch1_ch17, 0))    
        self.connect((self.fir_filter_xxx_0, 0), (self.qtgui_waterfall_sink_x_0, 0))    
        self.connect((self.fir_filter_xxx_0_0, 0), (self.pfb_channelizer_ch8_ch14, 0))    
        self.connect((self.fir_filter_xxx_0_0, 0), (self.qtgui_waterfall_sink_x_0_0, 0))    
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.fir_filter_xxx_0, 0))    
        self.connect((self.freq_xlating_fir_filter_xxx_0_0, 0), (self.fir_filter_xxx_0_0, 0))    
        self.connect((self.osmosdr_source_0, 0), (self.rx_enable_valve, 0))    
        self.connect((self.pfb_channelizer_ch1_ch17, 7), (self.blocks_null_sink_0_1_0, 0))    
        self.connect((self.pfb_channelizer_ch1_ch17, 8), (self.blocks_null_sink_0_1_0, 1))    
        self.connect((self.pfb_channelizer_ch1_ch17, 9), (self.blocks_null_sink_0_1_0, 2))    
        self.connect((self.pfb_channelizer_ch1_ch17, 10), (self.blocks_null_sink_0_1_0, 3))    
        self.connect((self.pfb_channelizer_ch1_ch17, 11), (self.blocks_null_sink_0_1_0, 4))    
        self.connect((self.pfb_channelizer_ch1_ch17, 12), (self.blocks_null_sink_0_1_0, 5))    
        self.connect((self.pfb_channelizer_ch1_ch17, 13), (self.blocks_null_sink_0_1_0, 6))    
        self.connect((self.pfb_channelizer_ch1_ch17, 14), (self.blocks_null_sink_0_1_0, 7))    
        self.connect((self.pfb_channelizer_ch1_ch17, 15), (self.blocks_null_sink_0_1_0, 8))    
        self.connect((self.pfb_channelizer_ch1_ch17, 0), (self.rx_ch1_valve, 0))    
        self.connect((self.pfb_channelizer_ch1_ch17, 1), (self.rx_ch2_valve, 0))    
        self.connect((self.pfb_channelizer_ch1_ch17, 2), (self.rx_ch3_valve, 0))    
        self.connect((self.pfb_channelizer_ch1_ch17, 3), (self.rx_ch4_valve, 0))    
        self.connect((self.pfb_channelizer_ch1_ch17, 4), (self.rx_ch5_valve, 0))    
        self.connect((self.pfb_channelizer_ch1_ch17, 5), (self.rx_ch6_valve, 0))    
        self.connect((self.pfb_channelizer_ch1_ch17, 6), (self.rx_ch7_valve, 0))    
        self.connect((self.pfb_channelizer_ch8_ch14, 7), (self.blocks_null_sink_0_1_0_0, 0))    
        self.connect((self.pfb_channelizer_ch8_ch14, 8), (self.blocks_null_sink_0_1_0_0, 1))    
        self.connect((self.pfb_channelizer_ch8_ch14, 9), (self.blocks_null_sink_0_1_0_0, 2))    
        self.connect((self.pfb_channelizer_ch8_ch14, 10), (self.blocks_null_sink_0_1_0_0, 3))    
        self.connect((self.pfb_channelizer_ch8_ch14, 11), (self.blocks_null_sink_0_1_0_0, 4))    
        self.connect((self.pfb_channelizer_ch8_ch14, 12), (self.blocks_null_sink_0_1_0_0, 5))    
        self.connect((self.pfb_channelizer_ch8_ch14, 13), (self.blocks_null_sink_0_1_0_0, 6))    
        self.connect((self.pfb_channelizer_ch8_ch14, 14), (self.blocks_null_sink_0_1_0_0, 7))    
        self.connect((self.pfb_channelizer_ch8_ch14, 15), (self.blocks_null_sink_0_1_0_0, 8))    
        self.connect((self.pfb_channelizer_ch8_ch14, 2), (self.rx_ch10_valve, 0))    
        self.connect((self.pfb_channelizer_ch8_ch14, 3), (self.rx_ch11_valve, 0))    
        self.connect((self.pfb_channelizer_ch8_ch14, 4), (self.rx_ch12_valve, 0))    
        self.connect((self.pfb_channelizer_ch8_ch14, 5), (self.rx_ch13_valve, 0))    
        self.connect((self.pfb_channelizer_ch8_ch14, 6), (self.rx_ch14_valve, 0))    
        self.connect((self.pfb_channelizer_ch8_ch14, 0), (self.rx_ch8_valve, 0))    
        self.connect((self.pfb_channelizer_ch8_ch14, 1), (self.rx_ch9_valve, 0))    
        self.connect((self.rx_ch10_analog_ctcss_squelch_ff, 0), (self.ch10_audio_sel, 0))    
        self.connect((self.rx_ch10_analog_pwr_squelch, 0), (self.rx_ch10_nbfm, 0))    
        self.connect((self.rx_ch10_hpf_iir, 0), (self.rx_ch10_audio_sink, 0))    
        self.connect((self.rx_ch10_nbfm, 0), (self.ch10_audio_sel, 1))    
        self.connect((self.rx_ch10_nbfm, 0), (self.rx_ch10_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch10_valve, 0), (self.rx_ch10_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch11_analog_ctcss_squelch_ff, 0), (self.ch11_audio_sel, 0))    
        self.connect((self.rx_ch11_analog_pwr_squelch, 0), (self.rx_ch11_nbfm, 0))    
        self.connect((self.rx_ch11_hpf_iir, 0), (self.rx_ch11_audio_sink, 0))    
        self.connect((self.rx_ch11_nbfm, 0), (self.ch11_audio_sel, 1))    
        self.connect((self.rx_ch11_nbfm, 0), (self.rx_ch11_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch11_valve, 0), (self.rx_ch11_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch12_analog_ctcss_squelch_ff, 0), (self.ch12_audio_sel, 0))    
        self.connect((self.rx_ch12_analog_pwr_squelch, 0), (self.rx_ch12_nbfm, 0))    
        self.connect((self.rx_ch12_hpf_iir, 0), (self.rx_ch12_audio_sink, 0))    
        self.connect((self.rx_ch12_nbfm, 0), (self.ch12_audio_sel, 1))    
        self.connect((self.rx_ch12_nbfm, 0), (self.rx_ch12_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch12_valve, 0), (self.rx_ch12_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch13_analog_ctcss_squelch_ff, 0), (self.ch13_audio_sel, 0))    
        self.connect((self.rx_ch13_analog_pwr_squelch, 0), (self.rx_ch13_nbfm, 0))    
        self.connect((self.rx_ch13_hpf_iir, 0), (self.rx_ch13_audio_sink, 0))    
        self.connect((self.rx_ch13_nbfm, 0), (self.ch13_audio_sel, 1))    
        self.connect((self.rx_ch13_nbfm, 0), (self.rx_ch13_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch13_valve, 0), (self.rx_ch13_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch14_analog_ctcss_squelch_ff, 0), (self.ch14_audio_sel, 0))    
        self.connect((self.rx_ch14_analog_pwr_squelch, 0), (self.rx_ch14_nbfm, 0))    
        self.connect((self.rx_ch14_hpf_iir, 0), (self.rx_ch14_audio_sink, 0))    
        self.connect((self.rx_ch14_nbfm, 0), (self.ch14_audio_sel, 1))    
        self.connect((self.rx_ch14_nbfm, 0), (self.rx_ch14_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch14_valve, 0), (self.rx_ch14_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch1_analog_ctcss_squelch_ff, 0), (self.ch1_audio_sel, 0))    
        self.connect((self.rx_ch1_analog_pwr_squelch, 0), (self.rx_ch1_nbfm, 0))    
        self.connect((self.rx_ch1_hpf_iir, 0), (self.rx_ch1_audio_sink, 0))    
        self.connect((self.rx_ch1_nbfm, 0), (self.ch1_audio_sel, 1))    
        self.connect((self.rx_ch1_nbfm, 0), (self.rx_ch1_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch1_valve, 0), (self.rx_ch1_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch2_analog_ctcss_squelch_ff, 0), (self.ch2_audio_sel, 0))    
        self.connect((self.rx_ch2_analog_pwr_squelch, 0), (self.rx_ch2_nbfm, 0))    
        self.connect((self.rx_ch2_hpf_iir, 0), (self.rx_ch2_audio_sink, 0))    
        self.connect((self.rx_ch2_nbfm, 0), (self.ch2_audio_sel, 1))    
        self.connect((self.rx_ch2_nbfm, 0), (self.rx_ch2_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch2_valve, 0), (self.rx_ch2_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch3_analog_ctcss_squelch_ff, 0), (self.ch3_audio_sel, 0))    
        self.connect((self.rx_ch3_analog_pwr_squelch, 0), (self.rx_ch3_nbfm, 0))    
        self.connect((self.rx_ch3_hpf_iir, 0), (self.rx_ch3_audio_sink, 0))    
        self.connect((self.rx_ch3_nbfm, 0), (self.ch3_audio_sel, 1))    
        self.connect((self.rx_ch3_nbfm, 0), (self.rx_ch3_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch3_valve, 0), (self.rx_ch3_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch4_analog_ctcss_squelch_ff, 0), (self.ch4_audio_sel, 0))    
        self.connect((self.rx_ch4_analog_pwr_squelch, 0), (self.rx_ch4_nbfm, 0))    
        self.connect((self.rx_ch4_hpf_iir, 0), (self.rx_ch4_audio_sink, 0))    
        self.connect((self.rx_ch4_nbfm, 0), (self.ch4_audio_sel, 1))    
        self.connect((self.rx_ch4_nbfm, 0), (self.rx_ch4_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch4_valve, 0), (self.rx_ch4_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch5_analog_ctcss_squelch_ff, 0), (self.ch5_audio_sel, 0))    
        self.connect((self.rx_ch5_analog_pwr_squelch, 0), (self.rx_ch5_nbfm, 0))    
        self.connect((self.rx_ch5_hpf_iir, 0), (self.rx_ch5_audio_sink, 0))    
        self.connect((self.rx_ch5_nbfm, 0), (self.ch5_audio_sel, 1))    
        self.connect((self.rx_ch5_nbfm, 0), (self.rx_ch5_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch5_valve, 0), (self.rx_ch5_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch6_analog_ctcss_squelch_ff, 0), (self.ch6_audio_sel, 0))    
        self.connect((self.rx_ch6_analog_pwr_squelch, 0), (self.rx_ch6_nbfm, 0))    
        self.connect((self.rx_ch6_hpf_iir, 0), (self.rx_ch6_audio_sink, 0))    
        self.connect((self.rx_ch6_nbfm, 0), (self.ch6_audio_sel, 1))    
        self.connect((self.rx_ch6_nbfm, 0), (self.rx_ch6_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch6_valve, 0), (self.rx_ch6_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch7_analog_ctcss_squelch_ff, 0), (self.ch7_audio_sel, 0))    
        self.connect((self.rx_ch7_analog_pwr_squelch, 0), (self.rx_ch7_nbfm, 0))    
        self.connect((self.rx_ch7_hpf_iir, 0), (self.rx_ch7_audio_sink, 0))    
        self.connect((self.rx_ch7_nbfm, 0), (self.ch7_audio_sel, 1))    
        self.connect((self.rx_ch7_nbfm, 0), (self.rx_ch7_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch7_valve, 0), (self.rx_ch7_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch8_analog_ctcss_squelch_ff, 0), (self.ch8_audio_sel, 0))    
        self.connect((self.rx_ch8_analog_pwr_squelch, 0), (self.rx_ch8_nbfm, 0))    
        self.connect((self.rx_ch8_hpf_iir, 0), (self.rx_ch8_audio_sink, 0))    
        self.connect((self.rx_ch8_nbfm, 0), (self.ch8_audio_sel, 1))    
        self.connect((self.rx_ch8_nbfm, 0), (self.rx_ch8_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch8_valve, 0), (self.rx_ch8_analog_pwr_squelch, 0))    
        self.connect((self.rx_ch9_analog_ctcss_squelch_ff, 0), (self.ch9_audio_sel, 0))    
        self.connect((self.rx_ch9_analog_pwr_squelch, 0), (self.rx_ch9_nbfm, 0))    
        self.connect((self.rx_ch9_hpf_iir, 0), (self.rx_ch9_audio_sink, 0))    
        self.connect((self.rx_ch9_nbfm, 0), (self.ch9_audio_sel, 1))    
        self.connect((self.rx_ch9_nbfm, 0), (self.rx_ch9_analog_ctcss_squelch_ff, 0))    
        self.connect((self.rx_ch9_valve, 0), (self.rx_ch9_analog_pwr_squelch, 0))    
        self.connect((self.rx_enable_valve, 0), (self.blocks_complex_to_mag_squared_0, 0))    
        self.connect((self.rx_enable_valve, 0), (self.freq_xlating_fir_filter_xxx_0, 0))    
        self.connect((self.rx_enable_valve, 0), (self.freq_xlating_fir_filter_xxx_0_0, 0))    
        self.connect((self.single_pole_iir_filter_xx_0, 0), (self.blocks_keep_one_in_n_0, 0))    
        self.connect((self.tx_audio_fir, 0), (self.tx_audio_gain_mult, 0))    
        self.connect((self.tx_audio_gain_mult, 0), (self.tx_audio_gain_mult2, 0))    
        self.connect((self.tx_audio_gain_mult, 0), (self.tx_audio_src_sel, 0))    
        self.connect((self.tx_audio_gain_mult2, 0), (self.tx_ctcss_add, 0))    
        self.connect((self.tx_audio_src_sel, 0), (self.tx_nbfm, 0))    
        self.connect((self.tx_ch10_mute, 0), (self.tx_ch8_14_pfs, 2))    
        self.connect((self.tx_ch11_mute, 0), (self.tx_ch8_14_pfs, 3))    
        self.connect((self.tx_ch12_mute, 0), (self.tx_ch8_14_pfs, 4))    
        self.connect((self.tx_ch13_mute, 0), (self.tx_ch8_14_pfs, 5))    
        self.connect((self.tx_ch14_mute, 0), (self.tx_ch8_14_pfs, 6))    
        self.connect((self.tx_ch1_7_fir1, 0), (self.tx_ch1_7_fir2, 0))    
        self.connect((self.tx_ch1_7_fir2, 0), (self.blocks_multiply_xx_0, 1))    
        self.connect((self.tx_ch1_7_pfs, 0), (self.tx_post_ps_gain_ch1_7, 0))    
        self.connect((self.tx_ch1_mute, 0), (self.tx_ch1_7_pfs, 0))    
        self.connect((self.tx_ch2_mute, 0), (self.tx_ch1_7_pfs, 1))    
        self.connect((self.tx_ch3_mute, 0), (self.tx_ch1_7_pfs, 2))    
        self.connect((self.tx_ch4_mute, 0), (self.tx_ch1_7_pfs, 3))    
        self.connect((self.tx_ch5_mute, 0), (self.tx_ch1_7_pfs, 4))    
        self.connect((self.tx_ch6_mute, 0), (self.tx_ch1_7_pfs, 5))    
        self.connect((self.tx_ch7_mute, 0), (self.tx_ch1_7_pfs, 6))    
        self.connect((self.tx_ch8_14_fir1, 0), (self.tx_ch8_14_fir2, 0))    
        self.connect((self.tx_ch8_14_fir2, 0), (self.blocks_multiply_xx_0_0, 1))    
        self.connect((self.tx_ch8_14_pfs, 0), (self.tx_post_ps_gain_ch8_14, 0))    
        self.connect((self.tx_ch8_mute, 0), (self.tx_ch8_14_pfs, 0))    
        self.connect((self.tx_ch9_mute, 0), (self.tx_ch8_14_pfs, 1))    
        self.connect((self.tx_ctcss_add, 0), (self.tx_audio_src_sel, 1))    
        self.connect((self.tx_ctcss_tone, 0), (self.tx_ctcss_add, 1))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch10_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch11_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch12_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch13_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch14_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch1_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch2_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch3_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch4_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch5_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch6_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch7_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch8_mute, 0))    
        self.connect((self.tx_nbfm, 0), (self.tx_ch9_mute, 0))    
        self.connect((self.tx_post_ps_gain_ch1_7, 0), (self.tx_ch1_7_fir1, 0))    
        self.connect((self.tx_post_ps_gain_ch1_7, 0), (self.tx_ch1_7_waterfall, 0))    
        self.connect((self.tx_post_ps_gain_ch8_14, 0), (self.tx_ch8_14_fir1, 0))    
        self.connect((self.tx_post_ps_gain_ch8_14, 0), (self.tx_ch8_14_waterfall, 0))    
        self.connect((self.tx_valve, 0), (self.tx_audio_fir, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "bladeRF_frs")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()


    def get_tx_audio_sample_rate(self):
        return self.tx_audio_sample_rate

    def set_tx_audio_sample_rate(self, tx_audio_sample_rate):
        self.tx_audio_sample_rate = tx_audio_sample_rate
        self.set_tx_pfs_output_rate(16 * self.tx_audio_sample_rate)
        self.tx_ctcss_tone.set_sampling_freq(self.tx_audio_sample_rate)

    def get_tx_rf_freq(self):
        return self.tx_rf_freq

    def set_tx_rf_freq(self, tx_rf_freq):
        self.tx_rf_freq = tx_rf_freq
        self.set_tx_ch11_offset(467.6375e6 - self.tx_rf_freq)
        self.set_tx_ch4_offset(462.6375e6 - self.tx_rf_freq)
        self.tx_ch1_7_waterfall.set_frequency_range(self.tx_rf_freq + self.tx_ch4_offset, self.tx_pfs_output_rate)
        self.tx_ch8_14_waterfall.set_frequency_range(self.tx_rf_freq + self.tx_ch11_offset, self.tx_pfs_output_rate)
        self.osmosdr_sink_0.set_center_freq(self.tx_rf_freq, 0)

    def get_tx_pfs_output_rate(self):
        return self.tx_pfs_output_rate

    def set_tx_pfs_output_rate(self, tx_pfs_output_rate):
        self.tx_pfs_output_rate = tx_pfs_output_rate
        self.set_tx_rf_sample_rate(20 * self.tx_pfs_output_rate)
        self.tx_ch1_7_waterfall.set_frequency_range(self.tx_rf_freq + self.tx_ch4_offset, self.tx_pfs_output_rate)
        self.tx_ch8_14_waterfall.set_frequency_range(self.tx_rf_freq + self.tx_ch11_offset, self.tx_pfs_output_rate)

    def get_tx_ch9_enable(self):
        return self.tx_ch9_enable

    def set_tx_ch9_enable(self, tx_ch9_enable):
        self.tx_ch9_enable = tx_ch9_enable
        self.set_tx_num_enabled_ch8_14(sum([self.tx_ch8_enable, self.tx_ch9_enable, self.tx_ch10_enable, self.tx_ch11_enable, self.tx_ch12_enable, self.tx_ch13_enable, self.tx_ch14_enable]))
        self._tx_ch9_enable_callback(self.tx_ch9_enable)
        self.tx_ch9_mute.set_mute(bool(not self.tx_ch9_enable))

    def get_tx_ch8_enable(self):
        return self.tx_ch8_enable

    def set_tx_ch8_enable(self, tx_ch8_enable):
        self.tx_ch8_enable = tx_ch8_enable
        self.set_tx_num_enabled_ch8_14(sum([self.tx_ch8_enable, self.tx_ch9_enable, self.tx_ch10_enable, self.tx_ch11_enable, self.tx_ch12_enable, self.tx_ch13_enable, self.tx_ch14_enable]))
        self.tx_ch8_mute.set_mute(bool(not self.tx_ch8_enable))
        self._tx_ch8_enable_callback(self.tx_ch8_enable)

    def get_tx_ch7_enable(self):
        return self.tx_ch7_enable

    def set_tx_ch7_enable(self, tx_ch7_enable):
        self.tx_ch7_enable = tx_ch7_enable
        self.set_tx_num_enabled_ch1_7(sum([self.tx_ch1_enable, self.tx_ch2_enable, self.tx_ch3_enable, self.tx_ch4_enable, self.tx_ch5_enable, self.tx_ch6_enable, self.tx_ch7_enable]))
        self.tx_ch7_mute.set_mute(bool(not self.tx_ch7_enable))
        self._tx_ch7_enable_callback(self.tx_ch7_enable)

    def get_tx_ch6_enable(self):
        return self.tx_ch6_enable

    def set_tx_ch6_enable(self, tx_ch6_enable):
        self.tx_ch6_enable = tx_ch6_enable
        self.set_tx_num_enabled_ch1_7(sum([self.tx_ch1_enable, self.tx_ch2_enable, self.tx_ch3_enable, self.tx_ch4_enable, self.tx_ch5_enable, self.tx_ch6_enable, self.tx_ch7_enable]))
        self.tx_ch6_mute.set_mute(bool(not self.tx_ch6_enable))
        self._tx_ch6_enable_callback(self.tx_ch6_enable)

    def get_tx_ch5_enable(self):
        return self.tx_ch5_enable

    def set_tx_ch5_enable(self, tx_ch5_enable):
        self.tx_ch5_enable = tx_ch5_enable
        self.set_tx_num_enabled_ch1_7(sum([self.tx_ch1_enable, self.tx_ch2_enable, self.tx_ch3_enable, self.tx_ch4_enable, self.tx_ch5_enable, self.tx_ch6_enable, self.tx_ch7_enable]))
        self.tx_ch5_mute.set_mute(bool(not self.tx_ch5_enable))
        self._tx_ch5_enable_callback(self.tx_ch5_enable)

    def get_tx_ch4_enable(self):
        return self.tx_ch4_enable

    def set_tx_ch4_enable(self, tx_ch4_enable):
        self.tx_ch4_enable = tx_ch4_enable
        self.set_tx_num_enabled_ch1_7(sum([self.tx_ch1_enable, self.tx_ch2_enable, self.tx_ch3_enable, self.tx_ch4_enable, self.tx_ch5_enable, self.tx_ch6_enable, self.tx_ch7_enable]))
        self.tx_ch4_mute.set_mute(bool(not self.tx_ch4_enable))
        self._tx_ch4_enable_callback(self.tx_ch4_enable)

    def get_tx_ch3_enable(self):
        return self.tx_ch3_enable

    def set_tx_ch3_enable(self, tx_ch3_enable):
        self.tx_ch3_enable = tx_ch3_enable
        self.set_tx_num_enabled_ch1_7(sum([self.tx_ch1_enable, self.tx_ch2_enable, self.tx_ch3_enable, self.tx_ch4_enable, self.tx_ch5_enable, self.tx_ch6_enable, self.tx_ch7_enable]))
        self.tx_ch3_mute.set_mute(bool(not self.tx_ch3_enable))
        self._tx_ch3_enable_callback(self.tx_ch3_enable)

    def get_tx_ch2_enable(self):
        return self.tx_ch2_enable

    def set_tx_ch2_enable(self, tx_ch2_enable):
        self.tx_ch2_enable = tx_ch2_enable
        self.set_tx_num_enabled_ch1_7(sum([self.tx_ch1_enable, self.tx_ch2_enable, self.tx_ch3_enable, self.tx_ch4_enable, self.tx_ch5_enable, self.tx_ch6_enable, self.tx_ch7_enable]))
        self._tx_ch2_enable_callback(self.tx_ch2_enable)
        self.tx_ch2_mute.set_mute(bool(not self.tx_ch2_enable))

    def get_tx_ch1_enable(self):
        return self.tx_ch1_enable

    def set_tx_ch1_enable(self, tx_ch1_enable):
        self.tx_ch1_enable = tx_ch1_enable
        self.set_tx_num_enabled_ch1_7(sum([self.tx_ch1_enable, self.tx_ch2_enable, self.tx_ch3_enable, self.tx_ch4_enable, self.tx_ch5_enable, self.tx_ch6_enable, self.tx_ch7_enable]))
        self._tx_ch1_enable_callback(self.tx_ch1_enable)
        self.tx_ch1_mute.set_mute(bool(not self.tx_ch1_enable))

    def get_tx_ch14_enable(self):
        return self.tx_ch14_enable

    def set_tx_ch14_enable(self, tx_ch14_enable):
        self.tx_ch14_enable = tx_ch14_enable
        self.set_tx_num_enabled_ch8_14(sum([self.tx_ch8_enable, self.tx_ch9_enable, self.tx_ch10_enable, self.tx_ch11_enable, self.tx_ch12_enable, self.tx_ch13_enable, self.tx_ch14_enable]))
        self.tx_ch14_mute.set_mute(bool(not self.tx_ch14_enable))
        self._tx_ch14_enable_callback(self.tx_ch14_enable)

    def get_tx_ch13_enable(self):
        return self.tx_ch13_enable

    def set_tx_ch13_enable(self, tx_ch13_enable):
        self.tx_ch13_enable = tx_ch13_enable
        self.set_tx_num_enabled_ch8_14(sum([self.tx_ch8_enable, self.tx_ch9_enable, self.tx_ch10_enable, self.tx_ch11_enable, self.tx_ch12_enable, self.tx_ch13_enable, self.tx_ch14_enable]))
        self._tx_ch13_enable_callback(self.tx_ch13_enable)
        self.tx_ch13_mute.set_mute(bool(not self.tx_ch13_enable))

    def get_tx_ch12_enable(self):
        return self.tx_ch12_enable

    def set_tx_ch12_enable(self, tx_ch12_enable):
        self.tx_ch12_enable = tx_ch12_enable
        self.set_tx_num_enabled_ch8_14(sum([self.tx_ch8_enable, self.tx_ch9_enable, self.tx_ch10_enable, self.tx_ch11_enable, self.tx_ch12_enable, self.tx_ch13_enable, self.tx_ch14_enable]))
        self._tx_ch12_enable_callback(self.tx_ch12_enable)
        self.tx_ch12_mute.set_mute(bool(not self.tx_ch12_enable))

    def get_tx_ch11_enable(self):
        return self.tx_ch11_enable

    def set_tx_ch11_enable(self, tx_ch11_enable):
        self.tx_ch11_enable = tx_ch11_enable
        self.set_tx_num_enabled_ch8_14(sum([self.tx_ch8_enable, self.tx_ch9_enable, self.tx_ch10_enable, self.tx_ch11_enable, self.tx_ch12_enable, self.tx_ch13_enable, self.tx_ch14_enable]))
        self._tx_ch11_enable_callback(self.tx_ch11_enable)
        self.tx_ch11_mute.set_mute(bool(not self.tx_ch11_enable))

    def get_tx_ch10_enable(self):
        return self.tx_ch10_enable

    def set_tx_ch10_enable(self, tx_ch10_enable):
        self.tx_ch10_enable = tx_ch10_enable
        self.set_tx_num_enabled_ch8_14(sum([self.tx_ch8_enable, self.tx_ch9_enable, self.tx_ch10_enable, self.tx_ch11_enable, self.tx_ch12_enable, self.tx_ch13_enable, self.tx_ch14_enable]))
        self._tx_ch10_enable_callback(self.tx_ch10_enable)
        self.tx_ch10_mute.set_mute(bool(not self.tx_ch10_enable))

    def get_rx_audio_sample_rate(self):
        return self.rx_audio_sample_rate

    def set_rx_audio_sample_rate(self, rx_audio_sample_rate):
        self.rx_audio_sample_rate = rx_audio_sample_rate
        self.set_rx_ctcss_len(int(0.250 * self.rx_audio_sample_rate))

    def get_rf_rx_freq(self):
        return self.rf_rx_freq

    def set_rf_rx_freq(self, rf_rx_freq):
        self.rf_rx_freq = rf_rx_freq
        self.set_rx_ch4_offset(462.6375e6 - self.rf_rx_freq)
        self.set_rx_ch11_offset(467.6375e6 - self.rf_rx_freq)
        self.osmosdr_source_0.set_center_freq(self.rf_rx_freq, 0)
        self.qtgui_waterfall_sink_x_0.set_frequency_range(self.rf_rx_freq + self.rx_ch4_offset, self.rx_rf_sample_rate / 20)
        self.qtgui_waterfall_sink_x_0_0.set_frequency_range(self.rf_rx_freq + self.rx_ch11_offset, self.rx_rf_sample_rate / 20)

    def get_ctcss_freq(self):
        return self.ctcss_freq

    def set_ctcss_freq(self, ctcss_freq):
        self.ctcss_freq = ctcss_freq
        self.set_ctcss_enable((self.ctcss_freq != 0))
        self._ctcss_freq_callback(self.ctcss_freq)
        self.rx_ch2_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch3_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch4_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch5_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch6_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch7_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch10_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch9_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch14_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch13_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch11_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch12_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch1_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.rx_ch8_analog_ctcss_squelch_ff.set_frequency(self.ctcss_freq)
        self.tx_ctcss_tone.set_frequency(self.ctcss_freq)

    def get_tx_vga2_gain(self):
        return self.tx_vga2_gain

    def set_tx_vga2_gain(self, tx_vga2_gain):
        self.tx_vga2_gain = tx_vga2_gain
        self.osmosdr_sink_0.set_gain(self.tx_vga2_gain, 0)

    def get_tx_vga1_gain(self):
        return self.tx_vga1_gain

    def set_tx_vga1_gain(self, tx_vga1_gain):
        self.tx_vga1_gain = tx_vga1_gain
        self.osmosdr_sink_0.set_bb_gain(self.tx_vga1_gain, 0)

    def get_tx_rf_sample_rate(self):
        return self.tx_rf_sample_rate

    def set_tx_rf_sample_rate(self, tx_rf_sample_rate):
        self.tx_rf_sample_rate = tx_rf_sample_rate
        self.analog_sig_source_x_0.set_sampling_freq(self.tx_rf_sample_rate)
        self.analog_sig_source_x_0_0.set_sampling_freq(self.tx_rf_sample_rate)
        self.osmosdr_sink_0.set_sample_rate(self.tx_rf_sample_rate)

    def get_tx_ptt(self):
        return self.tx_ptt

    def set_tx_ptt(self, tx_ptt):
        self.tx_ptt = tx_ptt
        self.tx_valve.set_open(bool(not (self.tx_ptt or self.tx_cont)))

    def get_tx_pfs_taps(self):
        return self.tx_pfs_taps

    def set_tx_pfs_taps(self, tx_pfs_taps):
        self.tx_pfs_taps = tx_pfs_taps
        self.tx_ch8_14_pfs.set_taps((self.tx_pfs_taps))
        self.tx_ch1_7_pfs.set_taps((self.tx_pfs_taps))

    def get_tx_num_enabled_ch8_14(self):
        return self.tx_num_enabled_ch8_14

    def set_tx_num_enabled_ch8_14(self, tx_num_enabled_ch8_14):
        self.tx_num_enabled_ch8_14 = tx_num_enabled_ch8_14
        self.tx_post_ps_gain_ch8_14.set_k((1 if self.tx_num_enabled_ch8_14 == 0 else 16 * 0.95 / self.tx_num_enabled_ch8_14, ))

    def get_tx_num_enabled_ch1_7(self):
        return self.tx_num_enabled_ch1_7

    def set_tx_num_enabled_ch1_7(self, tx_num_enabled_ch1_7):
        self.tx_num_enabled_ch1_7 = tx_num_enabled_ch1_7
        self.tx_post_ps_gain_ch1_7.set_k((1 if self.tx_num_enabled_ch1_7 == 0 else 16 * 0.95 / self.tx_num_enabled_ch1_7, ))

    def get_tx_interp_fir2_taps(self):
        return self.tx_interp_fir2_taps

    def set_tx_interp_fir2_taps(self, tx_interp_fir2_taps):
        self.tx_interp_fir2_taps = tx_interp_fir2_taps
        self.tx_ch8_14_fir2.set_taps((self.tx_interp_fir2_taps))
        self.tx_ch1_7_fir2.set_taps((self.tx_interp_fir2_taps))

    def get_tx_interp_fir1_taps(self):
        return self.tx_interp_fir1_taps

    def set_tx_interp_fir1_taps(self, tx_interp_fir1_taps):
        self.tx_interp_fir1_taps = tx_interp_fir1_taps
        self.tx_ch8_14_fir1.set_taps((self.tx_interp_fir1_taps))
        self.tx_ch1_7_fir1.set_taps((self.tx_interp_fir1_taps))

    def get_tx_cont(self):
        return self.tx_cont

    def set_tx_cont(self, tx_cont):
        self.tx_cont = tx_cont
        self.tx_valve.set_open(bool(not (self.tx_ptt or self.tx_cont)))
        self._tx_cont_callback(self.tx_cont)

    def get_tx_ch_label(self):
        return self.tx_ch_label

    def set_tx_ch_label(self, tx_ch_label):
        self.tx_ch_label = tx_ch_label
        Qt.QMetaObject.invokeMethod(self._tx_ch_label_label, "setText", Qt.Q_ARG("QString", str(self.tx_ch_label)))

    def get_tx_ch4_offset(self):
        return self.tx_ch4_offset

    def set_tx_ch4_offset(self, tx_ch4_offset):
        self.tx_ch4_offset = tx_ch4_offset
        self.tx_ch1_7_waterfall.set_frequency_range(self.tx_rf_freq + self.tx_ch4_offset, self.tx_pfs_output_rate)
        self.analog_sig_source_x_0.set_frequency(self.tx_ch4_offset)

    def get_tx_ch11_offset(self):
        return self.tx_ch11_offset

    def set_tx_ch11_offset(self, tx_ch11_offset):
        self.tx_ch11_offset = tx_ch11_offset
        self.analog_sig_source_x_0_0.set_frequency(self.tx_ch11_offset)
        self.tx_ch8_14_waterfall.set_frequency_range(self.tx_rf_freq + self.tx_ch11_offset, self.tx_pfs_output_rate)

    def get_tx_audio_gain(self):
        return self.tx_audio_gain

    def set_tx_audio_gain(self, tx_audio_gain):
        self.tx_audio_gain = tx_audio_gain
        self.tx_audio_gain_mult.set_k((self.tx_audio_gain, ))

    def get_rx_xlating_fir_taps(self):
        return self.rx_xlating_fir_taps

    def set_rx_xlating_fir_taps(self, rx_xlating_fir_taps):
        self.rx_xlating_fir_taps = rx_xlating_fir_taps
        self.freq_xlating_fir_filter_xxx_0_0.set_taps((self.rx_xlating_fir_taps))
        self.freq_xlating_fir_filter_xxx_0.set_taps((self.rx_xlating_fir_taps))

    def get_rx_vga_gain(self):
        return self.rx_vga_gain

    def set_rx_vga_gain(self, rx_vga_gain):
        self.rx_vga_gain = rx_vga_gain
        self.osmosdr_source_0.set_bb_gain(self.rx_vga_gain, 0)

    def get_rx_squelch(self):
        return self.rx_squelch

    def set_rx_squelch(self, rx_squelch):
        self.rx_squelch = rx_squelch
        self.rx_ch1_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch2_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch4_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch5_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch6_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch7_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch8_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch9_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch10_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch11_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch12_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch13_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch14_analog_pwr_squelch.set_threshold(self.rx_squelch)
        self.rx_ch3_analog_pwr_squelch.set_threshold(self.rx_squelch)

    def get_rx_rf_sample_rate(self):
        return self.rx_rf_sample_rate

    def set_rx_rf_sample_rate(self, rx_rf_sample_rate):
        self.rx_rf_sample_rate = rx_rf_sample_rate
        self.osmosdr_source_0.set_sample_rate(self.rx_rf_sample_rate)
        self.blocks_keep_one_in_n_0.set_n(int(self.rx_rf_sample_rate / 30))
        self.qtgui_waterfall_sink_x_0.set_frequency_range(self.rf_rx_freq + self.rx_ch4_offset, self.rx_rf_sample_rate / 20)
        self.qtgui_waterfall_sink_x_0_0.set_frequency_range(self.rf_rx_freq + self.rx_ch11_offset, self.rx_rf_sample_rate / 20)

    def get_rx_power_squelch_alpha(self):
        return self.rx_power_squelch_alpha

    def set_rx_power_squelch_alpha(self, rx_power_squelch_alpha):
        self.rx_power_squelch_alpha = rx_power_squelch_alpha
        self.rx_ch1_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch2_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch4_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch5_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch6_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch7_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch8_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch9_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch10_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch11_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch12_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch13_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch14_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)
        self.rx_ch3_analog_pwr_squelch.set_alpha(self.rx_power_squelch_alpha)

    def get_rx_pc_taps(self):
        return self.rx_pc_taps

    def set_rx_pc_taps(self, rx_pc_taps):
        self.rx_pc_taps = rx_pc_taps
        self.pfb_channelizer_ch1_ch17.set_taps((self.rx_pc_taps))
        self.pfb_channelizer_ch8_ch14.set_taps((self.rx_pc_taps))

    def get_rx_lna_gain(self):
        return self.rx_lna_gain

    def set_rx_lna_gain(self, rx_lna_gain):
        self.rx_lna_gain = rx_lna_gain
        self._rx_lna_gain_callback(self.rx_lna_gain)
        self.osmosdr_source_0.set_gain(self.rx_lna_gain, 0)

    def get_rx_enable(self):
        return self.rx_enable

    def set_rx_enable(self, rx_enable):
        self.rx_enable = rx_enable
        self._rx_enable_callback(self.rx_enable)
        self.rx_enable_valve.set_open(bool(not self.rx_enable))

    def get_rx_dec_fir_taps(self):
        return self.rx_dec_fir_taps

    def set_rx_dec_fir_taps(self, rx_dec_fir_taps):
        self.rx_dec_fir_taps = rx_dec_fir_taps
        self.fir_filter_xxx_0_0.set_taps((self.rx_dec_fir_taps))
        self.fir_filter_xxx_0.set_taps((self.rx_dec_fir_taps))

    def get_rx_ctcss_squelch(self):
        return self.rx_ctcss_squelch

    def set_rx_ctcss_squelch(self, rx_ctcss_squelch):
        self.rx_ctcss_squelch = rx_ctcss_squelch
        self.rx_ch2_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch3_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch4_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch5_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch6_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch7_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch10_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch9_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch14_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch13_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch11_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch12_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch1_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)
        self.rx_ch8_analog_ctcss_squelch_ff.set_level(self.rx_ctcss_squelch)

    def get_rx_ctcss_len(self):
        return self.rx_ctcss_len

    def set_rx_ctcss_len(self, rx_ctcss_len):
        self.rx_ctcss_len = rx_ctcss_len

    def get_rx_ch_label(self):
        return self.rx_ch_label

    def set_rx_ch_label(self, rx_ch_label):
        self.rx_ch_label = rx_ch_label
        Qt.QMetaObject.invokeMethod(self._rx_ch_label_label, "setText", Qt.Q_ARG("QString", str(self.rx_ch_label)))

    def get_rx_ch9_enable(self):
        return self.rx_ch9_enable

    def set_rx_ch9_enable(self, rx_ch9_enable):
        self.rx_ch9_enable = rx_ch9_enable
        self._rx_ch9_enable_callback(self.rx_ch9_enable)
        self.rx_ch9_valve.set_open(bool(not self.rx_ch9_enable))

    def get_rx_ch8_enable(self):
        return self.rx_ch8_enable

    def set_rx_ch8_enable(self, rx_ch8_enable):
        self.rx_ch8_enable = rx_ch8_enable
        self._rx_ch8_enable_callback(self.rx_ch8_enable)
        self.rx_ch8_valve.set_open(bool(not self.rx_ch8_enable))

    def get_rx_ch7_enable(self):
        return self.rx_ch7_enable

    def set_rx_ch7_enable(self, rx_ch7_enable):
        self.rx_ch7_enable = rx_ch7_enable
        self._rx_ch7_enable_callback(self.rx_ch7_enable)
        self.rx_ch7_valve.set_open(bool(not self.rx_ch7_enable))

    def get_rx_ch6_enable(self):
        return self.rx_ch6_enable

    def set_rx_ch6_enable(self, rx_ch6_enable):
        self.rx_ch6_enable = rx_ch6_enable
        self._rx_ch6_enable_callback(self.rx_ch6_enable)
        self.rx_ch6_valve.set_open(bool(not self.rx_ch6_enable))

    def get_rx_ch5_enable(self):
        return self.rx_ch5_enable

    def set_rx_ch5_enable(self, rx_ch5_enable):
        self.rx_ch5_enable = rx_ch5_enable
        self._rx_ch5_enable_callback(self.rx_ch5_enable)
        self.rx_ch5_valve.set_open(bool(not self.rx_ch5_enable))

    def get_rx_ch4_offset(self):
        return self.rx_ch4_offset

    def set_rx_ch4_offset(self, rx_ch4_offset):
        self.rx_ch4_offset = rx_ch4_offset
        self.freq_xlating_fir_filter_xxx_0.set_center_freq(self.rx_ch4_offset)
        self.qtgui_waterfall_sink_x_0.set_frequency_range(self.rf_rx_freq + self.rx_ch4_offset, self.rx_rf_sample_rate / 20)

    def get_rx_ch4_enable(self):
        return self.rx_ch4_enable

    def set_rx_ch4_enable(self, rx_ch4_enable):
        self.rx_ch4_enable = rx_ch4_enable
        self._rx_ch4_enable_callback(self.rx_ch4_enable)
        self.rx_ch4_valve.set_open(bool(not self.rx_ch4_enable))

    def get_rx_ch3_enable(self):
        return self.rx_ch3_enable

    def set_rx_ch3_enable(self, rx_ch3_enable):
        self.rx_ch3_enable = rx_ch3_enable
        self._rx_ch3_enable_callback(self.rx_ch3_enable)
        self.rx_ch3_valve.set_open(bool(not self.rx_ch3_enable))

    def get_rx_ch2_enable(self):
        return self.rx_ch2_enable

    def set_rx_ch2_enable(self, rx_ch2_enable):
        self.rx_ch2_enable = rx_ch2_enable
        self._rx_ch2_enable_callback(self.rx_ch2_enable)
        self.rx_ch2_valve.set_open(bool(not self.rx_ch2_enable))

    def get_rx_ch1_enable(self):
        return self.rx_ch1_enable

    def set_rx_ch1_enable(self, rx_ch1_enable):
        self.rx_ch1_enable = rx_ch1_enable
        self._rx_ch1_enable_callback(self.rx_ch1_enable)
        self.rx_ch1_valve.set_open(bool(not self.rx_ch1_enable))

    def get_rx_ch14_enable(self):
        return self.rx_ch14_enable

    def set_rx_ch14_enable(self, rx_ch14_enable):
        self.rx_ch14_enable = rx_ch14_enable
        self._rx_ch14_enable_callback(self.rx_ch14_enable)
        self.rx_ch14_valve.set_open(bool(not self.rx_ch14_enable))

    def get_rx_ch13_enable(self):
        return self.rx_ch13_enable

    def set_rx_ch13_enable(self, rx_ch13_enable):
        self.rx_ch13_enable = rx_ch13_enable
        self._rx_ch13_enable_callback(self.rx_ch13_enable)
        self.rx_ch13_valve.set_open(bool(not self.rx_ch13_enable))

    def get_rx_ch12_enable(self):
        return self.rx_ch12_enable

    def set_rx_ch12_enable(self, rx_ch12_enable):
        self.rx_ch12_enable = rx_ch12_enable
        self._rx_ch12_enable_callback(self.rx_ch12_enable)
        self.rx_ch12_valve.set_open(bool(not self.rx_ch12_enable))

    def get_rx_ch11_offset(self):
        return self.rx_ch11_offset

    def set_rx_ch11_offset(self, rx_ch11_offset):
        self.rx_ch11_offset = rx_ch11_offset
        self.qtgui_waterfall_sink_x_0_0.set_frequency_range(self.rf_rx_freq + self.rx_ch11_offset, self.rx_rf_sample_rate / 20)

    def get_rx_ch11_enable(self):
        return self.rx_ch11_enable

    def set_rx_ch11_enable(self, rx_ch11_enable):
        self.rx_ch11_enable = rx_ch11_enable
        self._rx_ch11_enable_callback(self.rx_ch11_enable)
        self.rx_ch11_valve.set_open(bool(not self.rx_ch11_enable))

    def get_rx_ch10_enable(self):
        return self.rx_ch10_enable

    def set_rx_ch10_enable(self, rx_ch10_enable):
        self.rx_ch10_enable = rx_ch10_enable
        self._rx_ch10_enable_callback(self.rx_ch10_enable)
        self.rx_ch10_valve.set_open(bool(not self.rx_ch10_enable))

    def get_nbfm_tau(self):
        return self.nbfm_tau

    def set_nbfm_tau(self, nbfm_tau):
        self.nbfm_tau = nbfm_tau

    def get_ctcss_hpf_ff_taps(self):
        return self.ctcss_hpf_ff_taps

    def set_ctcss_hpf_ff_taps(self, ctcss_hpf_ff_taps):
        self.ctcss_hpf_ff_taps = ctcss_hpf_ff_taps
        self.rx_ch1_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch3_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch2_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch4_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch5_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch6_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch8_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch9_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch10_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch11_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch13_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch12_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch14_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch7_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))

    def get_ctcss_hpf_fb_taps(self):
        return self.ctcss_hpf_fb_taps

    def set_ctcss_hpf_fb_taps(self, ctcss_hpf_fb_taps):
        self.ctcss_hpf_fb_taps = ctcss_hpf_fb_taps
        self.rx_ch1_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch3_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch2_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch4_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch5_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch6_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch8_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch9_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch10_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch11_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch13_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch12_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch14_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))
        self.rx_ch7_hpf_iir.set_taps((self.ctcss_hpf_ff_taps), (self.ctcss_hpf_fb_taps))

    def get_ctcss_enable(self):
        return self.ctcss_enable

    def set_ctcss_enable(self, ctcss_enable):
        self.ctcss_enable = ctcss_enable
        self.tx_audio_src_sel.set_input_index(int(1 if self.ctcss_enable else 0))
        self.ch1_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch3_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch2_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch4_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch5_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch6_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch8_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch14_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch9_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch10_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch11_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch12_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch13_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))
        self.ch7_audio_sel.set_input_index(int(0 if self.ctcss_enable else 1))


def main(top_block_cls=bladeRF_frs, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
