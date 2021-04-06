import math
import numpy as np
import matplotlib.pyplot as plt

ln = 10000 #use small num for test
Vft = np.loadtxt('data/V-front.txt', max_rows=ln) #cell-volume
Cr3 = np.loadtxt('data/C-run2-600.txt', max_rows=ln)

ttVol = sum(Vft)
ttC = sum(Vft*Cr3)
aveC = ttC/ttVol
# std = np.sqrt(sum(Vft*(Cr3-aveC)*(Cr3-aveC))/ttVol)
minC = min(Cr3)
maxC = max(Cr3)

gamma = 50.
Q = 0.236
t = 600.
WR = gamma/Q*(1.-np.exp(-Q/ttVol*t)) #Wells-Riley

print ttVol, ttC, aveC, minC, maxC, WR

tmin = 50.
tmax = 300.
ns = 50
dc = (tmax-tmin)/ns
gc = np.zeros(ns) #histogram

for i in range(ln):
	ni = int(math.floor((Cr3[i]-tmin)/dc))
	ni = max(0, min(ns-1, ni))
	gc[ni] += Vft[i]

fc = gc/(sum(gc)*dc) #normalize histogram
nc = np.linspace(tmin, tmax, ns) #normalized C for x-axis

plt.rcParams.update({'font.size': 20})
plt.figure(figsize=(10, 6))
plt.xlim([tmin, tmax])
plt.ylim([-1e-4, 7.5e-2])
# plt.xscale('log')
plt.plot([WR, WR], [-1, 1], 'k-', linewidth=3, label='Well-mixed model')
plt.plot(nc, fc, 'r-', label='RANS simulation')
plt.tick_params(which='both', direction='in')
plt.tick_params(which='both', bottom=True, top=True, left=True, right=True)
plt.tick_params(which='both', labelbottom=True, labeltop=False, labelleft=True, labelright=False)
plt.subplots_adjust(left=0.12, bottom=0.1, right=0.96, top=0.96, wspace=0, hspace=0)
plt.xlabel('C')
plt.ylabel('PDF(C)')
# plt.title('Run 3, T = 900s')
plt.legend(loc='best', fontsize=20, frameon=False)
plt.show()

