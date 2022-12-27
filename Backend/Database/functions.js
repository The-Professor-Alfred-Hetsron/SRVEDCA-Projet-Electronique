function toMinutes(date){
    let tab = moment(date).format('HH:mm').split(':').map((value) => Number(value))
    return 60 * tab[0] + tab[1]
}

module.exports = {toMinutes}