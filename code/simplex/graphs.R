#
# produce some nice graphs from measurements of Simplex simulations
#
# (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
d <- read.csv("computetime.csv", header = TRUE);
m = mean(d[d$m > 1000,]$length)

pdf("pathlength.pdf", 8, 6);

plot(0, 0, type = "n",
	main = "Simplex path length",
	xlab = "number of inequalities",
	ylab = "simplex path length",
	xlim = c(0, 9000), ylim = c(2, 3))
points(d$m, d$length, type = "p")
abline(m, 0, col = "red", lwd = 2)

pdf("pathstep.pdf", 8, 6)

plot(0, 0, type = "n",
	main = "Simplex steps to solution",
	xlab = "number of inequalities",
	ylab = "simplex steps to solution",
	xlim = c(0,9000), ylim = c(0, 80))
points(d$m, d$step, type = "p")


d$logm = log10(d$m)
d$logsteps = log10(d$steps)
d$logtime = log10(d$runtime)

l <- lm(logsteps ~ logm, d)
lt <- lm(logtime ~ logm, d)
icpt = coefficients(summary(l))[1,1]

pdf("pathsteplog.pdf", 8, 6)

plot(xy.coords(d$m, d$step), type = "p", log="xy",
	main = "Simplex steps to solution",
	xlab = "number of inequalities",
	ylab = "simplex steps to solution")
abline(l, col = "red", lwd = 2)
grid()

pdf("pathsteploghyp.pdf", 8, 6)

plot(xy.coords(d$m, d$step), type = "p", log="xy",
	main = "Simplex steps to solution",
	xlab = "number of inequalities",
	ylab = "simplex steps to solution")
abline(l, col = "red", lwd = 2)
abline(a = icpt, b = 0.5, col = "blue", lwd = 2)
grid()

pdf("runtime.pdf", 8, 6)
plot(xy.coords(d$m, d$runtime), type = "p", log="xy",
	main = "Simplex algorithm run time",
	xlab = "number of inequalities",
	ylab = "simplex runtime [s]")
abline(lt, col = "red", lwd = 2)
