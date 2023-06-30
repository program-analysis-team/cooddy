#include <pthread.h>

struct xnet_map_conn_mgr_t {
    pthread_mutex_t conn_map_lock;
};

struct xnet_net_ctrler_t {
    xnet_map_conn_mgr_t conn_mgr;
};

static inline void xnet_pthread_mutex_lock(pthread_mutex_t *mutex_lock) {
    (void)pthread_mutex_lock((mutex_lock));

}

static inline void xnet_pthread_mutex_unlock(pthread_mutex_t *mutex_lock) {
    (void)pthread_mutex_unlock((mutex_lock));
}

void xnet_net_ctrler_conn_map_lock(xnet_map_conn_mgr_t *map_conn_mgr) {
    xnet_pthread_mutex_lock(&(map_conn_mgr->conn_map_lock));
}

void xnet_net_ctrler_conn_map_unlock(xnet_map_conn_mgr_t *map_conn_mgr) {
    xnet_pthread_mutex_unlock(&(map_conn_mgr->conn_map_lock));
}

void xnet_net_ctrler_close_listen_sub_conns(xnet_net_ctrler_t *net_ctrler) {
    xnet_net_ctrler_conn_map_unlock(&(net_ctrler->conn_mgr));
    xnet_net_ctrler_conn_map_unlock(&(net_ctrler->conn_mgr));
    return;
}

int xnet_net_ctrl_work_inst_stop_listening(xnet_net_ctrler_t *net_ctrler) {
    xnet_net_ctrler_close_listen_sub_conns(net_ctrler);
    return 0;
}
