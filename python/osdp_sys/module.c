/*
 * Copyright (c) 2020-2024 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "module.h"
#include <stdio.h>

static PyModuleDef osdp_sys_module = {
	PyModuleDef_HEAD_INIT,
	.m_name = "osdp_sys",
	.m_doc = "Open Supervised Device Protocol",
	.m_size = -1,
};

void pyosdp_add_module_constants(PyObject *module)
{
#define ADD_CONST(k, v) PyModule_AddIntConstant(module, k, v);

	/* setup flags */
	ADD_CONST("FLAG_ENFORCE_SECURE", OSDP_FLAG_ENFORCE_SECURE);
	ADD_CONST("FLAG_INSTALL_MODE", OSDP_FLAG_INSTALL_MODE);
	ADD_CONST("FLAG_IGN_UNSOLICITED", OSDP_FLAG_IGN_UNSOLICITED);
	ADD_CONST("FLAG_ENABLE_NOTIFICATION", OSDP_FLAG_ENABLE_NOTIFICATION);
	ADD_CONST("FLAG_CAPTURE_PACKETS", OSDP_FLAG_CAPTURE_PACKETS);
	ADD_CONST("FLAG_ALLOW_EMPTY_ENCRYPTED_DATA_BLOCK", OSDP_FLAG_ALLOW_EMPTY_ENCRYPTED_DATA_BLOCK);

	ADD_CONST("LOG_EMERG", OSDP_LOG_EMERG);
	ADD_CONST("LOG_ALERT", OSDP_LOG_ALERT);
	ADD_CONST("LOG_CRIT", OSDP_LOG_CRIT);
	ADD_CONST("LOG_ERROR", OSDP_LOG_ERROR);
	ADD_CONST("LOG_WARNING", OSDP_LOG_WARNING);
	ADD_CONST("LOG_NOTICE", OSDP_LOG_NOTICE);
	ADD_CONST("LOG_INFO", OSDP_LOG_INFO);
	ADD_CONST("LOG_DEBUG", OSDP_LOG_DEBUG);
	ADD_CONST("LOG_MAX_LEVEL", OSDP_LOG_MAX_LEVEL);

	/* enum osdp_cmd_e */
	ADD_CONST("CMD_OUTPUT", OSDP_CMD_OUTPUT);
	ADD_CONST("CMD_LED", OSDP_CMD_LED);
	ADD_CONST("CMD_BUZZER", OSDP_CMD_BUZZER);
	ADD_CONST("CMD_TEXT", OSDP_CMD_TEXT);
	ADD_CONST("CMD_COMSET", OSDP_CMD_COMSET);
	ADD_CONST("CMD_COMSET_DONE", OSDP_CMD_COMSET_DONE);
	ADD_CONST("CMD_KEYSET", OSDP_CMD_KEYSET);
	ADD_CONST("CMD_MFG", OSDP_CMD_MFG);
	ADD_CONST("CMD_FILE_TX", OSDP_CMD_FILE_TX);
	ADD_CONST("CMD_STATUS", OSDP_CMD_STATUS);

	ADD_CONST("STATUS_REPORT_LOCAL", OSDP_STATUS_REPORT_LOCAL)
	ADD_CONST("STATUS_REPORT_INPUT", OSDP_STATUS_REPORT_INPUT)
	ADD_CONST("STATUS_REPORT_OUTPUT", OSDP_STATUS_REPORT_OUTPUT)
	ADD_CONST("STATUS_REPORT_REMOTE", OSDP_STATUS_REPORT_REMOTE)

	/* For `struct osdp_cmd_file_tx::flags` */
	ADD_CONST("CMD_FILE_TX_FLAG_CANCEL", OSDP_CMD_FILE_TX_FLAG_CANCEL);

	/* For `struct osdp_event_notification::type` */
	ADD_CONST("EVENT_NOTIFICATION_COMMAND", OSDP_EVENT_NOTIFICATION_COMMAND);
	ADD_CONST("EVENT_NOTIFICATION_SC_STATUS", OSDP_EVENT_NOTIFICATION_SC_STATUS);
	ADD_CONST("EVENT_NOTIFICATION_PD_STATUS", OSDP_EVENT_NOTIFICATION_PD_STATUS);

	/* enum osdp_event_type */
	ADD_CONST("EVENT_CARDREAD", OSDP_EVENT_CARDREAD);
	ADD_CONST("EVENT_KEYPRESS", OSDP_EVENT_KEYPRESS);
	ADD_CONST("EVENT_MFGREP", OSDP_EVENT_MFGREP);
	ADD_CONST("EVENT_STATUS", OSDP_EVENT_STATUS);
	ADD_CONST("EVENT_NOTIFICATION", OSDP_EVENT_NOTIFICATION);

	/* enum osdp_led_color_e */
	ADD_CONST("LED_COLOR_NONE", OSDP_LED_COLOR_NONE);
	ADD_CONST("LED_COLOR_RED", OSDP_LED_COLOR_RED);
	ADD_CONST("LED_COLOR_GREEN", OSDP_LED_COLOR_GREEN);
	ADD_CONST("LED_COLOR_AMBER", OSDP_LED_COLOR_AMBER);
	ADD_CONST("LED_COLOR_BLUE", OSDP_LED_COLOR_BLUE);
	ADD_CONST("LED_COLOR_MAGENTA", OSDP_LED_COLOR_MAGENTA);
	ADD_CONST("LED_COLOR_CYAN", OSDP_LED_COLOR_CYAN);
	ADD_CONST("LED_COLOR_WHITE", OSDP_LED_COLOR_WHITE);

	/* enum osdp_event_cardread_format_e */
	ADD_CONST("CARD_FMT_RAW_UNSPECIFIED", OSDP_CARD_FMT_RAW_UNSPECIFIED);
	ADD_CONST("CARD_FMT_RAW_WIEGAND", OSDP_CARD_FMT_RAW_WIEGAND);
	ADD_CONST("CARD_FMT_ASCII", OSDP_CARD_FMT_ASCII);

	/* enum osdp_pd_cap_function_code_e */
	ADD_CONST("CAP_UNUSED", OSDP_PD_CAP_UNUSED);
	ADD_CONST("CAP_CONTACT_STATUS_MONITORING",
		  OSDP_PD_CAP_CONTACT_STATUS_MONITORING);
	ADD_CONST("CAP_OUTPUT_CONTROL", OSDP_PD_CAP_OUTPUT_CONTROL);
	ADD_CONST("CAP_CARD_DATA_FORMAT", OSDP_PD_CAP_CARD_DATA_FORMAT);
	ADD_CONST("CAP_READER_LED_CONTROL", OSDP_PD_CAP_READER_LED_CONTROL);
	ADD_CONST("CAP_READER_AUDIBLE_OUTPUT",
		  OSDP_PD_CAP_READER_AUDIBLE_OUTPUT);
	ADD_CONST("CAP_READER_TEXT_OUTPUT", OSDP_PD_CAP_READER_TEXT_OUTPUT);
	ADD_CONST("CAP_TIME_KEEPING", OSDP_PD_CAP_TIME_KEEPING);
	ADD_CONST("CAP_CHECK_CHARACTER_SUPPORT",
		  OSDP_PD_CAP_CHECK_CHARACTER_SUPPORT);
	ADD_CONST("CAP_COMMUNICATION_SECURITY",
		  OSDP_PD_CAP_COMMUNICATION_SECURITY);
	ADD_CONST("CAP_RECEIVE_BUFFERSIZE", OSDP_PD_CAP_RECEIVE_BUFFERSIZE);
	ADD_CONST("CAP_LARGEST_COMBINED_MESSAGE_SIZE",
		  OSDP_PD_CAP_LARGEST_COMBINED_MESSAGE_SIZE);
	ADD_CONST("CAP_SMART_CARD_SUPPORT", OSDP_PD_CAP_SMART_CARD_SUPPORT);
	ADD_CONST("CAP_READERS", OSDP_PD_CAP_READERS);
	ADD_CONST("CAP_BIOMETRICS", OSDP_PD_CAP_BIOMETRICS);

#undef ADD_CONST
}

#define pyosdp_set_loglevel_doc                                                \
	"Set OSDP logging level\n"                                             \
	"\n"                                                                   \
	"@param log_level OSDP log level (0 to 7)\n"                           \
	"\n"                                                                   \
	"@return None"
static PyObject *pyosdp_set_loglevel(void *self, PyObject *args)
{
	int log_level = OSDP_LOG_DEBUG;

	if (!PyArg_ParseTuple(args, "I", &log_level)) {
		PyErr_SetString(PyExc_KeyError, "invalid log level");
		return NULL;
	}

	if (log_level < OSDP_LOG_EMERG ||
	    log_level > OSDP_LOG_MAX_LEVEL) {
		PyErr_SetString(PyExc_KeyError, "invalid log level");
		return NULL;
	}

	osdp_logger_init("pyosdp", log_level, NULL);

	Py_RETURN_NONE;
}

static PyMethodDef pyosdp_nodule_methods[] = {
	{ "set_loglevel", (PyCFunction)pyosdp_set_loglevel, METH_VARARGS,
	  pyosdp_set_loglevel_doc },
	{ NULL, NULL, 0, NULL } /* Sentinel */
};

PyMODINIT_FUNC PyInit_osdp_sys(void)
{
	PyObject *module;

	module = PyModule_Create(&osdp_sys_module);
	if (module == NULL)
		return NULL;

	pyosdp_add_module_constants(module);

	PyModule_AddFunctions(module, pyosdp_nodule_methods);

	do {

		if (pyosdp_add_type_osdp_base(module))
			break;

		if (pyosdp_add_type_cp(module))
			break;

		if (pyosdp_add_type_pd(module))
			break;

		return module;

	} while (0);

	Py_DECREF(module);
	return NULL;
}
