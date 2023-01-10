const mongoose = require('mongoose')

const audioSchema = new mongoose.Schema({
	name: String,
	desc: String,
	audio:
	{
		data: Buffer,
		contentType: String, //Exemple: 'audio/wav'
	}
})

module.exports = new mongoose.model('Audio', audioSchema)
