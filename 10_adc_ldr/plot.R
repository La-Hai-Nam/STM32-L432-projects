#! /usr/bin/Rscript
vals<-read.table("vals.tsv")
factor<-3.6/4096
plot(vals, type="l",xlab="Time",ylab="Value")
pdf("vals.pdf")
plot(vals[,1],vals[,2]*factor,type="l",xlab="Time",ylab="Voltage")
dev.off()
