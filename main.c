/* Function for disconnecting from the mqttsn gateway. */
static int discon(void) {
    int res = emcute_discon();
    if (res == EMCUTE_NOGW) {
        puts("error: not connected to any broker");
        return 1;
    }
    else if (res != EMCUTE_OK) {
        puts("error: unable to disconnect");
        return 1;
    }
    puts("Disconnect successful");
    return 0;
}

/* Function for publishing data.
 * The first argument is the string of the topic,
 * the second argument is the message to send,
 * the third argument is the QoS level.
 */
static int pub(char *topic, char *data, int qos) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    switch (qos) {
        case 1:
            flags |= EMCUTE_QOS_1;
            break;
        case 2:
            flags |= EMCUTE_QOS_2;
            break;
        default:
            flags |= EMCUTE_QOS_0;
            break;
    }


    /* Step 1: get topic id */
    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    /* Step 2: publish data */
    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
               t.name, (int)t.id);
        return 1;
    }

    printf("published %s on topic %s\n", data, topic);
    return 0;
}

/* Function for connecting to the mqttsn gateway.
 * The first argument is the address the second is the port.
 */
static int con(char *addr, int port) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    gw.port = port;

    /* Parse address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, addr) == NULL) {
        printf("error parsing IPv6 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", addr, port);
        return 1;
    }

    printf("Successfully connected to gateway at [%s]:%i\n", addr, port);
    return 0;
}
