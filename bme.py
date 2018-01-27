import syslog
import weewx
from weewx.wxengine import StdService

class bme(StdService):
    def __init__(self, engine, config_dict):
        super(bme, self).__init__(engine, config_dict)
        d = config_dict.get('PondService', {})
        self.filename = d.get('filename', '/home/weewx/public_html/data/bme.txt')
        syslog.syslog(syslog.LOG_INFO, "bme: using %s" % self.filename)
        self.bind(weewx.NEW_ARCHIVE_RECORD, self.read_file)

    def read_file(self, event):
        try:
            with open(self.filename) as f:
                #value = f.read()
                line = f.readline()
                value = line.split(',')
            syslog.syslog(syslog.LOG_DEBUG, "bme: found value of %s" % value)
            event.record['extraTemp1'] = float(value[0])
            event.record['extraHumid1'] = float(value[1])
            //event.record['soilTemp3'] = float(value[2])
            //event.record['soilTemp4'] = float(value[3])
        except Exception, e:
            syslog.syslog(syslog.LOG_ERR, "bme: cannot read value: %s" % e)
