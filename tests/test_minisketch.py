from fixtures import *  # noqa: F401,F403
from utils import mine_funding_to_announce
import time
import pytest


# experimental features too
@pytest.mark.developer("needs --dev-fast-gossip")
def test_minisketch_decoding(node_factory, bitcoind):
    l1, l2, l3 = node_factory.get_nodes(3, opts={'dev-fast-gossip-prune': None,
                                                 'log-level': 'debug:gossipd'})
    l1.rpc.connect(l2.info['id'], 'localhost', l2.port)
    bitcoind.generate_block(1)
    l2.rpc.connect(l3.info['id'], 'localhost', l3.port)
    scid12, _ = l1.fundchannel(l2, 10**6)
    scid23, _ = l2.fundchannel(l3, 10**6)
    mine_funding_to_announce(bitcoind, [l1, l2, l3])
    bitcoind.generate_block(1)
    l3.daemon.wait_for_log('Received node_announcement for node ' + l1.info['id'])
    time.sleep(.2)  # do we get anything else interesting after this?
