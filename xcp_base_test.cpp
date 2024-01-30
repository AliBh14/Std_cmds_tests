TEST(BaseTest, std_commands)
{
    memset_port(tx_done_msgs, 0, sizeof(tx_done_msgs));
    tx_msgs_cnt = 0;
    tx_msgs_done_cnt = 0;
    uint8_t cmd_tx_idx = 0;
    uint8_t mode = 0;
    uint8_t ag = 0;
    uint8_t seg = 0;
    uint8_t page = 0;

    /**********************************************/
    /* xcp standard mandatory commands tests      */
    /**********************************************/
    /* 
     *  First master should connect to the slave device
     */
    /* XCP connect commanad test */
    mode = 0;
    uint8_t connect_frame[] = { XCP_CONNECT_STD_CMD_PID, mode };
    receive_emulation(p_mcu, 2, connect_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(8, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL((xcp_init_test.xcp_module_feat_sup.xcp_cal_pag_feat_supported) << 0 | (xcp_init_test.xcp_module_feat_sup.xcp_daq_feat_supported) << 2 | (xcp_init_test.xcp_module_feat_sup.xcp_stim_feat_supported) << 3 | (xcp_init_test.xcp_module_feat_sup.xcp_pgm_feat_supported) << 4, tx_done_msgs[cmd_tx_idx].data[1]);
    BYTES_EQUAL(MCU_ENDIANESS << 0 | 0 << 1 | (xcp_init_test.xcp_module_feat_sup.xcp_blk_mode_feat_supported) << 6 | 0 << 7, tx_done_msgs[cmd_tx_idx].data[2]);
    BYTES_EQUAL(xcp_init_test.xcp_module_macro_cfg.xcp_max_cto, tx_done_msgs[cmd_tx_idx].data[3]);
    BYTES_EQUAL(xcp_init_test.xcp_module_macro_cfg.xcp_max_dto & 0xFF, tx_done_msgs[cmd_tx_idx].data[4]);
    BYTES_EQUAL(xcp_init_test.xcp_module_macro_cfg.xcp_max_dto >> 8, tx_done_msgs[cmd_tx_idx].data[5]);
    BYTES_EQUAL(XCP_PROTOCOL_MAJOR_VERSION << 4, tx_done_msgs[cmd_tx_idx].data[6]);
    BYTES_EQUAL(XCP_TRANSPORT_MAJOR_VERSION << 4, tx_done_msgs[cmd_tx_idx].data[7]);

    /* XCP get status commanad test */
    cmd_tx_idx++;
    uint8_t get_status_frame[] = { XCP_GET_STATUS_STD_CMD_PID };
    receive_emulation(p_mcu, 1, get_status_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(6, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[1]); 
    if (xcp_init_test.xcp_module_feat_sup.xcp_prot_feat_supported)
    {
        BYTES_EQUAL(xcp_init_test.xcp_module_spc_cfg_vars.xcp_prot_cfg, tx_done_msgs[2].data[2]); 
    }
    else
    {
        BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[2]);
    }
    BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[4]); 
    BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[5]);

    /* XCP synch commanad test */
    cmd_tx_idx++;
    uint8_t synch_frame[] = { XCP_SYNCH_STD_CMD_PID };
    receive_emulation(p_mcu, 1, synch_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(2, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_ERR, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(XCP_ERR_CMD_SYNCH, tx_done_msgs[cmd_tx_idx].data[1]);

    /**********************************************/
    /* xcp standard optional commands tests */
    /**********************************************/

    /* XCP get communication mode info commanad test */
    cmd_tx_idx++;
    uint8_t get_comm_mode_info_frame[] = { XCP_GET_COMM_MODE_INFO_STD_CMD_PID };
    receive_emulation(p_mcu, 1, get_comm_mode_info_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(8, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(xcp_init_test.xcp_module_feat_sup.xcp_blk_mode_feat_supported | 0 << 1, tx_done_msgs[cmd_tx_idx].data[2]);
    BYTES_EQUAL(pdur_target_xcp_tx_test.mem_pool.nblks, tx_done_msgs[cmd_tx_idx].data[4]);
    BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[5]);
    BYTES_EQUAL(pdur_target_xcp_tx_test.mem_pool.nblks, tx_done_msgs[cmd_tx_idx].data[6]);
    BYTES_EQUAL(XCP_PROTOCOL_MAJOR_VERSION << 4 | XCP_PROTOCOL_MINOR_VERSION, tx_done_msgs[cmd_tx_idx].data[7]);

    cmd_tx_idx++;
    uint8_t id_type = 0; /* ASCII text */
    mode = 0;
    uint8_t get_id_frame[] = { XCP_GET_ID_STD_CMD_PID, id_type };
    receive_emulation(p_mcu, 2, get_id_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(8, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(mode, tx_done_msgs[cmd_tx_idx].data[1]);
    BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[2]);
    BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[3]);
    uint32_t txt_len = strlen(xcp_init_test.xcp_module_spc_cfg_vars.xcp_info.xcp_ascii_txt);
    BYTES_EQUAL(txt_len & 0xFF, tx_done_msgs[cmd_tx_idx].data[4]);
    BYTES_EQUAL((txt_len >> 8) & 0xFF, tx_done_msgs[cmd_tx_idx].data[5]);
    BYTES_EQUAL((txt_len >> 16) & 0xFF, tx_done_msgs[cmd_tx_idx].data[6]);
    BYTES_EQUAL((txt_len >> 24) & 0xFF, tx_done_msgs[cmd_tx_idx].data[7]);

    cmd_tx_idx++;
    ag = txt_len;
    uint8_t upload_frame[] = { XCP_UPLOAD_STD_CMD_PID, ag };
    receive_emulation(p_mcu, 2, upload_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL((txt_len + 1), tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL('c', tx_done_msgs[cmd_tx_idx].data[1]);
    BYTES_EQUAL('a', tx_done_msgs[cmd_tx_idx].data[2]);
    BYTES_EQUAL('p', tx_done_msgs[cmd_tx_idx].data[3]);

    /* XCP set request commanad test */
    cmd_tx_idx++;
    mode = 0;  /* Invalid mode */
    uint16_t session_config_id = 0x1452;
    uint8_t set_requeset_frame[] = { XCP_SET_REQ_STD_CMD_PID, mode, (session_config_id & 0xff), ((session_config_id >> 8) & 0xff) };
    receive_emulation(p_mcu, 4, set_requeset_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(2, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_ERR, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(XCP_ERR_OUT_OF_RANGE, tx_done_msgs[cmd_tx_idx].data[1]);

    /* XCP upload commanad test */
    cmd_tx_idx++;
    ag = 4;
    upload_frame[1] = ag;
    //to-do: test block mode later.
    receive_emulation(p_mcu, 2, upload_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(5, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(0x78, tx_done_msgs[cmd_tx_idx].data[1]);
    BYTES_EQUAL(0x56, tx_done_msgs[cmd_tx_idx].data[2]);
    BYTES_EQUAL(0x34, tx_done_msgs[cmd_tx_idx].data[3]);
    BYTES_EQUAL(0x12, tx_done_msgs[cmd_tx_idx].data[4]);

    /* XCP short upload commanad test */
    cmd_tx_idx++;
    ag = 4;
    uint8_t short_upload_frame[] = { XCP_SHORT_UPLOAD_STD_CMD_PID, ag, 0, 0, addr & 0xff, (addr >> 8) & 0xff, (addr >> 16) & 0xff, addr >> 24 };
    receive_emulation(p_mcu, 8, short_upload_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(5, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(0x78, tx_done_msgs[cmd_tx_idx].data[1]);
    BYTES_EQUAL(0x56, tx_done_msgs[cmd_tx_idx].data[2]);
    BYTES_EQUAL(0x34, tx_done_msgs[cmd_tx_idx].data[3]);
    BYTES_EQUAL(0x12, tx_done_msgs[cmd_tx_idx].data[4]);

    /* XCP build checksum commanad test */
    cmd_tx_idx++;
    ag = 4;
    uint8_t build_checksum_frame[] = { XCP_BUILD_CHECKSUM_STD_CMD_PID, 0, 0, 0, ag, 0, 0, 0 };
    receive_emulation(p_mcu, 8, build_checksum_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(8, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(1, tx_done_msgs[cmd_tx_idx].data[1]); 14 for type 1
    BYTES_EQUAL(0x14, tx_done_msgs[cmd_tx_idx].data[4]);
    BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[5]);
    BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[6]);
    BYTES_EQUAL(0, tx_done_msgs[cmd_tx_idx].data[7]);

    /**********************************************/
    /* xcp standard auxiliary commands tests */
    /**********************************************/

    /* XCP TRANSPORT_LAYER_CMD commanad test */
    cmd_tx_idx++;
    mode = 0;
    uint8_t txp_layer_frame[] = { XCP_TRANSPORT_LAYER_CMD_STD_CMD_PID, 0xFF, 0x58, 0x43, 0x50, mode };
    receive_emulation(p_mcu, 8, txp_layer_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(5, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL('X', tx_done_msgs[cmd_tx_idx].data[1]);
    BYTES_EQUAL('C', tx_done_msgs[cmd_tx_idx].data[2]);
    BYTES_EQUAL('P', tx_done_msgs[cmd_tx_idx].data[3]);
    BYTES_EQUAL(mode, tx_done_msgs[cmd_tx_idx].data[4]);

    mode = 1;
    txp_layer_frame[5] = mode;
    cmd_tx_idx++;
    receive_emulation(p_mcu, 8, txp_layer_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(5, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(~'X', tx_done_msgs[cmd_tx_idx].data[1]);
    BYTES_EQUAL(~'C', tx_done_msgs[cmd_tx_idx].data[2]);
    BYTES_EQUAL(~'P', tx_done_msgs[cmd_tx_idx].data[3]);
    BYTES_EQUAL(mode, tx_done_msgs[cmd_tx_idx].data[4]);

    /*invalid mode test case in TRANSPORT Layer Command*/
    mode = 2;
    txp_layer_frame[5] = mode;
    cmd_tx_idx++;
    receive_emulation(p_mcu, 8, txp_layer_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(2, tx_done_msgs[cmd_tx_idx].info.dlc);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(XCP_PID_ERR, tx_done_msgs[cmd_tx_idx].data[0]);
    BYTES_EQUAL(XCP_ERR_OUT_OF_RANGE, tx_done_msgs[cmd_tx_idx].data[1]);

    /* XCP disconnect commanad test */
    cmd_tx_idx++;
    uint8_t disconnect_frame[] = { XCP_DISCONNECT_STD_CMD_PID, 0, 0, 0, 0, 0, 0, 0 };
    receive_emulation(p_mcu, 1, disconnect_frame);
    LONGS_EQUAL(xcp_tx_id, tx_done_msgs[cmd_tx_idx].info.id);
    BYTES_EQUAL(1, tx_done_msgs[cmd_tx_idx].info.dlc);
    BYTES_EQUAL(XCP_PID_RES, tx_done_msgs[cmd_tx_idx].data[0]);
}
