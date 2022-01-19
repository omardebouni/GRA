#include "custom_math.h"
#include "inverse_sinh.h"
#include <math.h>

#define M_E         2.71828182845904523536028747135266250
#define ITERATIONS 25

double ln_table[] = {0.0, 0.009950330853168092, 0.01980262729617973, 0.02955880224154443, 0.03922071315328133, 0.04879016416943205, 0.058268908123975824, 0.06765864847381486, 0.0769610411361284, 0.08617769624105241, 0.09531017980432493, 0.10436001532424286, 0.11332868530700327, 0.1222176327242493, 0.1310282624064042, 0.13976194237515882, 0.1484200051182734, 0.1570037488096649, 0.16551443847757352, 0.17395330712343815, 0.1823215567939548, 0.19062035960864987, 0.19885085874516534, 0.2070141693843263, 0.21511137961694568, 0.22314355131420993, 0.2311117209633868, 0.2390169004705001, 0.24686007793152598, 0.2546422183735809, 0.2623642644674913, 0.2700271372130604, 0.2776317365982797, 0.28517894223366264, 0.2926696139628202, 0.3001045924503383, 0.3074846997479609, 0.31481073984003377, 0.32208349916911355, 0.32930374714260063, 0.33647223662121317, 0.3435897043900772, 0.3506568716131696, 0.35767444427181616, 0.3646431135879095, 0.3715635564324833, 0.3784364357202453, 0.3852624007906452, 0.392042087776024, 0.39877611995736806, 0.40546510810816466, 0.41210965082683326, 0.4187103348581853, 0.4252677354043444, 0.4317824164255381, 0.4382549309311556, 0.444685821261446, 0.451075619360217, 0.45742484703887576, 0.4637340162321405, 0.4700036292457359, 0.47623417899637194, 0.48242614924429306, 0.4885800148186713, 0.4946962418361074, 0.5007752879124896, 0.5068176023684522, 0.5128236264286641, 0.518793793415168, 0.5247285289349825, 0.5306282510621707, 0.5364933705145688, 0.542324290825362, 0.5481214085096879, 0.5538851132264381, 0.5596157879354231, 0.5653138090500608, 0.5709795465857381, 0.5766133643039941, 0.582215619852664, 0.5877866649021194, 0.5933268452777348, 0.5988365010887043, 0.60431596685333, 0.6097655716208946, 0.6151856390902338, 0.6205764877251103, 0.6259384308664957, 0.6312717768418582, 0.6365768290715514, 0.6418538861723952, 0.647103242058539, 0.6523251860396906, 0.6575200029167946, 0.6626879730752372, 0.6678293725756559, 0.6729444732424263, 0.6780335427498976, 0.6830968447064443, 0.6881346387364015, 0.6931471805599457, 0.6981347220709847, 0.7030975114131138, 0.7080357930536966, 0.7129498078561255, 0.7178397931503172, 0.7227059828014901, 0.7275486072772783, 0.7323678937132271, 0.73716406597672, 0.7419373447293778, 0.7466879474879757, 0.7514160886839215, 0.7561219797213341, 0.7608058290337606, 0.765467842139572, 0.7701082216960741, 0.7747271675523686, 0.7793248768009982, 0.78390154382841, 0.7884573603642706, 0.7929925155296619, 0.7975071958841886, 0.8020015854720279, 0.806475865866949, 0.8109302162163292, 0.815364813284195, 0.8197798314933119, 0.8241754429663499, 0.8285518175661487, 0.8329091229351046, 0.8372475245337028, 0.8415671856782191, 0.8458682675776096, 0.8501509293696106, 0.8544153281560682, 0.8586616190375193, 0.8628899551470403, 0.8671004876833839, 0.8712933659434199, 0.8754687373539004, 0.8796267475025641, 0.8837675401685956, 0.8878912573524577, 0.891998039305111, 0.896088024556636, 0.900161349944272, 0.9042181506398864, 0.9082585601768913, 0.9122827104766167, 0.9162907318741556, 0.9202827531436931, 0.9242589015233325, 0.9282193027394293, 0.9321640810304458, 0.9360933591703354, 0.9400072584914716, 0.9439058989071288, 0.9477893989335267, 0.951657875711447, 0.9555114450274369, 0.9593502213346025, 0.963174317773006, 0.9669838461896738, 0.9707789171582254, 0.9745596399981313, 0.9783261227936083, 0.9820784724121588, 0.9858167945227658, 0.9895411936137483, 0.993251773010284, 0.9969486348916101, 1.0006318803079066, 1.0043016091968688, 1.0079579203999793, 1.0116009116784805, 1.0152306797290591, 1.0188473201992476, 1.0224509277025462, 1.026041595833275, 1.0296194171811588, 1.033184483345655, 1.0367368849500227, 1.040276711655147, 1.0438040521731153, 1.0473189942805596, 1.050821624831762, 1.0543120297715305, 1.0577902941478552, 1.0612565021243412, 1.064710736992429, 1.068153081183402, 1.071583616280191, 1.0750024230289765, 1.0784095813505907, 1.081805170351729, 1.0851892683359696, 1.0885619528146087, 1.0919233005173137, 1.0952733874025957, 1.0986122886681102, 1.101940078760785, 1.1052568313867788, 1.1085626195212785, 1.111857515418131, 1.1151415906193207, 1.11841491596429, 1.1216775615991064, 1.1249295969854838, 1.1281710909096547, 1.1314021114911013, 1.1346227261911435, 1.1378330018213916, 1.1410330045520625, 1.1442227999201626, 1.1474024528375424, 1.1505720275988214, 1.1537315878891898, 1.156881196792086, 1.160020916796754, 1.1631508098056815, 1.166270937141925, 1.1693813595563176, 1.172482137234566, 1.1755733298042386, 1.1786549963416466, 1.1817271953786168, 1.1847899849091628, 1.187843422396053, 1.190887564777281, 1.1939224684724352, 1.1969481893889722, 1.1999647829283977, 1.202972303992353, 1.2059708069886097, 1.2089603458369758, 1.2119409739751135, 1.2149127443642709, 1.217875709494928, 1.2208299213923597, 1.2237754316221163, 1.226712291295426, 1.2296405510745143, 1.2325602611778492, 1.2354714713853077, 1.238374231043269, 1.2412685890696336, 1.2441545939587686, 1.2470322937863836, 1.2499017362143363, 1.2527629684953687, 1.2556160374777752, 1.2584609896100063, 1.261297870945206, 1.2641267271456837, 1.266947603487325, 1.2697605448639397, 1.2725655957915483, 1.2753628004126096, 1.2781522025001881, 1.280933845462065, 1.28370777234479, 1.2864740258376803, 1.2892326482767602, 1.2919836816486499, 1.2947271675944008, 1.2974631474132756, 1.3001916620664795, 1.3029127521808401, 1.3056264580524364, 1.3083328196501793, 1.3110318766193445, 1.313723668285056, 1.3164082336557248, 1.3190856114264413, 1.3217558399823202, 1.3244189574018037, 1.32707500145992, 1.329724009631497, 1.3323660190943356, 1.3350010667323406, 1.3376291891386103, 1.3402504226184844, 1.3428648031925554, 1.3454723665996362, 1.3480731482996935, 1.35066718347674, 1.3532545070416913, 1.3558351536351827, 1.3584091576303556, 1.3609765531356015, 1.3635373739972751, 1.3660916538023717, 1.3686394258811705, 1.3711807233098432, 1.3737155789130313, 1.37624402526639, 1.3787660946990996, 1.381281819296347, 1.3837912309017728, 1.3862943611198912, 1.3887912413184784, 1.3912819026309304, 1.3937663759585923, 1.3962446919730593, 1.3987168811184485, 1.4011829736136419, 1.4036429994545043, 1.406096988416071, 1.408544970054711, 1.410986973710263, 1.413423028508144, 1.4158531633614357, 1.418277406972942, 1.4206957878372235, 1.4231083342426076, 1.4255150742731726, 1.4279160358107108, 1.4303112465366656, 1.4327007339340472, 1.4350845252893234, 1.4374626476942907, 1.439835128047921, 1.4422019930581873, 1.444563269243867, 1.446918982936326, 1.44926916028128, 1.451613827240534, 1.453953009593706, 1.4562867329399263, 1.4586150226995174, 1.4609379041156572, 1.4632554022560196, 1.4655675420143992, 1.4678743481123142, 1.4701758451005935, 1.4724720573609436, 1.4747630091074995, 1.4770487243883554, 1.4793292270870808, 1.4816045409242162, 1.4838746894587553, 1.4861396960896076, 1.488399584057045, 1.490654376444134, 1.4929040961781495, 1.4951487660319733, 1.4973884086254783, 1.4996230464268945, 1.5018527017541636, 1.5040773967762748, 1.5062971535145877, 1.5085119938441405, 1.5107219394949434, 1.5129270120532572, 1.5151272329628598, 1.5173226235262953, 1.519513204906114, 1.5216989981260944, 1.5238800240724546, 1.52605630349505, 1.528227857008558, 1.5303947050936482, 1.5325568680981434, 1.5347143662381646, 1.5368672195992656, 1.5390154481375553, 1.5411590716808068, 1.543298109929556, 1.5454325824581887, 1.5475625087160134, 1.5496879080283272, 1.5518087995974648, 1.5539252025038424, 1.556037135706986, 1.5581446180465506, 1.5602476682433293, 1.5623463049002504, 1.5644405465033653, 1.5665304114228247, 1.568615917913846, 1.5706970841176704, 1.5727739280625097, 1.574846467664482, 1.576914720728541, 1.5789787049493924, 1.581038437912403, 1.5830939370944992, 1.5851452198650564, 1.5871923034867814, 1.5892352051165817, 1.59127394180643, 1.5933085305042174, 1.5953389880545994, 1.5973653311998317, 1.5993875765805996, 1.6014057407368367, 1.603419840108538, 1.6054298910365623, 1.607435909763428, 1.6094379124341012, 1.611435915096774, 1.6134299337036384, 1.6154199841116486, 1.6174060820832779, 1.619388243287269, 1.621366483299375, 1.6233408176030926, 1.6253112615903913, 1.627277830562432, 1.6292405397302807, 1.6311994042156137, 1.6331544390514172, 1.635105659182679, 1.6370530794670746, 1.6389967146756454, 1.640936579493472, 1.6428726885203384, 1.6448050562713923, 1.646733697177798, 1.6486586255873823, 1.6505798557652762, 1.6524974018945482, 1.6544112780768323, 1.6563214983329515, 1.658228076603533, 1.6601310267496192, 1.6620303625532717, 1.663926097718171, 1.6658182458702087, 1.667706820558077, 1.6695918352538481, 1.6714733033535538, 1.6733512381777538, 1.6752256529721041, 1.677096560907916, 1.6789639750827114, 1.680827908520774, 1.6826883741736938, 1.6845453849209064, 1.6863989535702295, 1.6882490928583909, 1.6900958154515555, 1.691939133945845, 1.693779060867852, 1.6956156086751535, 1.6974487897568145, 1.6992786164338907, 1.701105100959925, 1.70292825552144, 1.704748092238426, 1.706564623164824, 1.7083778602890047, 1.710187815534244, 1.7119945007591932, 1.713797927758344, 1.7155981082624918, 1.7173950539391933, 1.7191887763932203, 1.7209792871670087, 1.7227665977411042, 1.7245507195346057, 1.7263316639056006, 1.7281094421515997, 1.7298840655099683, 1.7316555451583502, 1.7334238922150922, 1.7351891177396614, 1.7369512327330607, 1.7387102481382402, 1.7404661748405053, 1.7422190236679196, 1.7439688053917073, 1.745715530726649, 1.7474592103314757, 1.7491998548092598, 1.7509374747078006, 1.752672080520009, 1.7544036826842868, 1.7561322915849047, 1.7578579175523743, 1.7595805708638206, 1.761300261743347, 1.7630170003624017, 1.7647307968401365, 1.766441661243766, 1.7681496035889221, 1.7698546338400059, 1.7715567619105363, 1.773255997663496, 1.7749523509116745, 1.7766458314180076, 1.778336448895915, 1.7800242130096349, 1.7817091333745543, 1.7833912195575392, 1.785070481077259, 1.7867469274045114, 1.7884205679625411, 1.7900914121273588, 1.7917594692280558, 1.7934247485471169, 1.7950872593207305, 1.7967470107390948, 1.7984040119467244, 1.8000582720427507, 1.8017098000812237, 1.8033586050714077, 1.8050046959780763, 1.8066480817218065, 1.8082887711792663, 1.8099267731835047, 1.8115620965242354, 1.8131947499481207, 1.8148247421590518, 1.8164520818184273, 1.8180767775454292, 1.8196988379172971, 1.8213182714696001, 1.8229350866965055, 1.8245492920510467, 1.826160895945388, 1.8277699067510889, 1.8293763327993626, 1.830980182381337, 1.8325814637483109, 1.8341801851120079, 1.83577635464483, 1.837369980480108, 1.8389610707123498, 1.8405496333974878, 1.8421356765531225, 1.8437192081587668, 1.8453002361560855, 1.8468787684491355, 1.848454812904601, 1.8500283773520316, 1.8515994695840727, 1.8531680973566993, 1.8547342683894443, 1.856297990365627, 1.8578592709325794, 1.8594181177018707, 1.8609745382495289, 1.862528540116263, 1.8640801308076818, 1.8656293177945111, 1.86717610851281, 1.8687205103641842, 1.8702625307159992, 1.8718021769015922, 1.8733394562204786, 1.8748743759385624, 1.8764069432883403, 1.877937165469108, 1.8794650496471614, 1.8809906029559984, 1.8825138324965198, 1.8840347453372266, 1.8855533485144167, 1.8870696490323806, 1.8885836538635956, 1.8900953699489176, 1.891604804197772, 1.8931119634883433, 1.8946168546677635, 1.8961194845522986, 1.8976198599275331, 1.899117987548555, 1.9006138741401377, 1.9021075263969212, 1.903598950983591, 1.9050881545350589, 1.9065751436566372, 1.9080599249242165, 1.9095425048844392, 1.9110228900548736, 1.9125010869241843, 1.9139771019523049, 1.9154509415706056, 1.9169226121820617, 1.9183921201614218, 1.9198594718553716, 1.9213246735826994, 1.92278773163446, 1.9242486522741347, 1.9257074417377946, 1.9271641062342588, 1.928618651945253, 1.930071085025568, 1.9315214116032144, 1.9329696377795793, 1.934415769629579, 1.9358598132018126, 1.9373017745187138, 1.9387416595767015, 1.940179474346329, 1.9416152247724332, 1.943048916774282, 1.9444805562457197, 1.9459101490553141, 1.9473377010464994, 1.9487632180377203, 1.9501867058225744, 1.9516081701699517, 1.953027616824178, 1.9544450515051515, 1.9558604799084822, 1.9572739077056291, 1.9586853405440368, 1.9600947840472704, 1.9615022438151504, 1.9629077254238851, 1.9643112344262053, 1.9657127763514939, 1.967112356705917, 1.968509980972555, 1.9699056546115297, 1.9712993830601335, 1.972691171732956, 1.9740810260220103, 1.9754689512968584, 1.9768549529047357, 1.978239036170674, 1.9796212063976257, 1.9810014688665842, 1.9823798288367054, 1.9837562915454288, 1.9851308622085955, 1.9865035460205676, 1.9878743481543464, 1.9892432737616879, 1.990610327973221, 1.9919755158985608, 1.993338842626425, 1.994700313224746, 1.9960599327407857, 1.997417706201246, 1.998773638612382, 2.0001277349601114, 2.0014800002101247, 2.002830439307996, 2.00417905717929, 2.0055258587296687, 2.006870848845001, 2.008214032391469, 2.0095554142156704, 2.010894999144727, 2.0122327919863867, 2.0135687975291288, 2.0149030205422656, 2.016235465776044, 2.017566137961749, 2.018895041811803, 2.0202221820198654, 2.0215475632609343, 2.0228711901914425, 2.024193067449358, 2.025513199654281, 2.0268315914075394, 2.028148247292286, 2.029463171873595, 2.0307763696985557, 2.0320878452963664, 2.0333976031784298, 2.0347056478384453, 2.0360119837525006, 2.0373166153791655, 2.0386195471595814, 2.0399207835175535, 2.041220328859639, 2.0425181875752387, 2.0438143640366855, 2.0451088625993314, 2.0464016876016364, 2.0476928433652564, 2.048982334195128, 2.050270164379557, 2.0515563381903013, 2.0528408598826577, 2.0541237336955467, 2.055404963851595, 2.0566845545572203, 2.0579625100027124, 2.059238834362317, 2.0605135317943177, 2.061786606441116, 2.0630580624293127, 2.0643279038697884, 2.065596134857784, 2.0668627594729765, 2.0681277817795634, 2.069391205826335, 2.070653035646757, 2.0719132752590452, 2.073171928666241, 2.0744289998562926, 2.0756844928021243, 2.076938411461718, 2.078190759778184, 2.0794415416798366, 2.0806907610802683, 2.081938421878424, 2.083184527958671, 2.0844290831908756, 2.0856720914304727, 2.0869135565185375, 2.088153482281858, 2.0893918725330045, 2.0906287310704013, 2.091864061678394, 2.0930978681273222, 2.0943301541735875, 2.09556092355972, 2.0967901800144495, 2.098017927252772, 2.0992441689760164, 2.100468908871912, 2.1016921506146566, 2.102913897864979, 2.1041341542702083, 2.1053529234643378, 2.106570209068089, 2.1077860146889793, 2.109000343921381, 2.1102132003465903, 2.1114245875328876, 2.1126345090356007, 2.113842968397169, 2.1150499691472042, 2.116255514802553, 2.1174596088673576, 2.118662254833118, 2.119863456178752, 2.1210632163706564, 2.122261538862765, 2.1234584270966113, 2.124653884501385, 2.1258479144939924, 2.127040520479116, 2.128231705849269, 2.1294214739848574, 2.130609828254236, 2.131796772013765, 2.1329823086078665, 2.134166441369083, 2.1353491736181325, 2.136530508663964, 2.1377104498038126, 2.138889000323257, 2.1400661634962717, 2.1412419425852836, 2.1424163408412253, 2.1435893615035884, 2.1447610078004793, 2.14593128294867, 2.1471001901536515, 2.148267732609689, 2.149433913499872, 2.150598735996165, 2.1517622032594628, 2.15292431843964, 2.1540850846756023, 2.1552445050953373, 2.156402582815965, 2.1575593209437884, 2.1587147225743446, 2.1598687907924514, 2.1610215286722596, 2.1621729392773013, 2.1633230256605387, 2.1644717908644124, 2.1656192379208887, 2.166765369851512, 2.167910189667445, 2.169053700369524, 2.170195904948301, 2.1713368063840925, 2.172476407647026, 2.1736147116970863, 2.174751721484162, 2.175887439948089, 2.177021870018701, 2.1781550146158692, 2.179286876649553, 2.180417459019839, 2.18154676461699, 2.182674796321489, 2.183801557004079, 2.184927049525814, 2.186051276738095, 2.187174241482719, 2.1882959465919187, 2.1894163948884082, 2.1905355891854237, 2.1916535322867645, 2.19277022698684, 2.1938856760707055, 2.194999882314109, 2.1961128484835295, 2.19722457733622, 2.198335071620247, 2.199444334074533, 2.200552367428895, 2.2016591744040857, 2.2027647577118357, 2.2038691200548888, 2.204972264127046, 2.206074192613203, 2.2071749081893883, 2.208274413522805, 2.2093727112718677, 2.210469804086241, 2.211565694606878, 2.2126603854660596, 2.213753879287431, 2.21484617868604, 2.215937286268374, 2.2170272046323998, 2.2181159363675964, 2.2192034840549955, 2.2202898502672164, 2.2213750375685035, 2.2224590485147617, 2.2235418856535936, 2.2246235515243344, 2.2257040486580886, 2.2267833795777645, 2.22786154679811, 2.228938552825748, 2.2300144001592113, 2.2310890912889767, 2.2321626286975005, 2.233235014859253, 2.234306252240752, 2.2353763433005964, 2.2364452904895016, 2.2375130962503316, 2.238579763018134, 2.2396452932201725, 2.240709689275959, 2.2417729535972892, 2.2428350885882726, 2.2438960966453667, 2.2449559801574104, 2.246014741505652, 2.2470723830637875, 2.2481289071979877, 2.2491843162669314, 2.250238612621837, 2.2512917986064958, 2.2523438765573, 2.2533948488032745, 2.2544447176661118, 2.255493485460196, 2.2565411544926395, 2.257587727063311, 2.2586332054648635, 2.25967759198277, 2.2607208888953476, 2.2617630984737915, 2.262804222982202, 2.263844264677616, 2.264883225810035, 2.265921108622455, 2.266957915350895, 2.267993648224427, 2.2690283094652037, 2.270061901288486, 2.2710944259026755, 2.272125885509338, 2.2731562823032343, 2.2741856184723486, 2.2752138961979145, 2.2762411176544446, 2.2772672850097564, 2.278292400425002, 2.2793164660546923, 2.280339484046727, 2.28136145654242, 2.282382385676527, 2.2834022735772725, 2.2844211223663753, 2.285438934159076, 2.286455711064163, 2.2874714551839985, 2.288486168614545, 2.289499853445391, 2.2905125117597773, 2.2915241456346216, 2.2925347571405448, 2.2935443483418974, 2.2945529212967823, 2.295560478057082, 2.2965670206684834, 2.2975725511705023, 2.2985770715965077, 2.2995805839737478, 2.3005830903233733, 2.301584592660463};

/*Die Funktion implementiert die Formel für ein bestimmtes x mithilfe einer reinen
Reihendarstellung und gibt das Ergebnis zurück
https://proofwiki.org/wiki/Power_Series_Expansion_for_Real_Area_Hyperbolic_Sine */
double approxArsinh_series(double x) {
    double absX = customAbs(x);
    double result = 0;
    double dividend, divisor;
    if (absX <= 1) {
        for (int n = 0; n < ITERATIONS; n++) {
            dividend = sign(n) * customFactorial(2 * n) * customPow(x, 2 * n + 1);
            divisor = customPow(2, 2 * n) * customPow(customFactorial(n), 2) * (2 * n + 1);
            result += (dividend / divisor);
        }
    } else if (x >= 1) {
        for (int n = 1; n < ITERATIONS; n++) {
            dividend = sign(n) * customFactorial(2 * n);
            divisor = customPow(2, 2 * n) * customPow(customFactorial(n), 2) * (2 * n) * customPow(x, 2 * n);
            result += (dividend / divisor);
        }
        result = customLn(2 * x) - result;
    } else if (x <= -1) {
        for (int n = 0; n < ITERATIONS; n++) {
            dividend = customPow(-1, n) * customFactorial(2 * n);
            divisor = customPow(2, 2 * n) * customPow(customFactorial(n), 2) * (2 * n) * customPow(x, 2 * n);
            if (divisor != 0) {
                result += (dividend / divisor);
            }
        }
        result = -customLn(-2 * x) + result;
    }
    return result;
}

/**
 * This is an auxiliary function for the lookup function.
 * It expects only values of which the natural logarithm
 * is precalculated and saved in the lookup table array (between 1.00 and 9.99).
 * Returns the index of the sought logarithm as an int
 */
int index_of(double x) {
    return (int) customFloor((x - 1) * 100);
}

/**
 * This is an auxiliary function for the lookup function.
 * It returns the logarithm of the closest value to the
 * given x parameter. The values returned are between 1.00 and 9.99
 * with a step size of 0.01
 */
double get_closest_entry(double x) {
    x *= 100;
    double floored = customFloor(x);
    if ((x - floored) < 0.5) return floored / 100;
    else return (floored + 1) / 100;
}

// TODO: implement ln using a lookup table
double lookup_ln(double x) {
    if (x == 0) return -INFINITY;
    if (x < 1) return -1 + lookup_ln(x * M_E);
    else if (x > 10) return 1 + lookup_ln(x / M_E);
    
    //recursion base
    if (x <= 1.01) return ln_table[index_of(get_closest_entry(x))];
        

    double closest_entry = get_closest_entry(x);
    return ln_table[index_of(closest_entry)] + lookup_ln(1 + customAbs(x - closest_entry)/closest_entry);
}

/*Die Funktion implementiert die Formel für ein bestimmtes x mithilfe eines Tabellen-
Lookups.*/
double approxArsinh_lookup(double x) {
    x += customSqrt(customPow(x, 2) + 1); // x + sqrt(x^2 + 1)
    return lookup_ln(x); // ln(x + sqrt(x^2 + 1))
}
