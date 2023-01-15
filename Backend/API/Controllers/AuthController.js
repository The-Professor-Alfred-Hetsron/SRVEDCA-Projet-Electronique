const Admin = require('../../Database/Admin')
const bcrypt = require('bcrypt')
const jwt = require('jsonwebtoken')

const secret = "SRVEDCA-Projet-Electronique";

module.exports.signin = async (req, res, next) => {
  const { email, motdepasse } = req.body;
  

  try {
    const oldUser = await Admin.findOne({ email });

    if (!oldUser) return res.status(404).json({ message: "User doesn't exist" });

    const isPasswordCorrect = await bcrypt.compare(motdepasse, oldUser.motdepasse);

    if (!isPasswordCorrect) return res.status(400).json({ message: "Invalid credentials" });

    const token = jwt.sign({ email: oldUser.email, id: oldUser._id }, secret, { expiresIn: "720h" });

    res.status(200).json({ result: oldUser, token });
  } catch (err) {
    res.status(500).json({ message: "Something went wrong", content: err.message });
  }
};

module.exports.signup = async (req, res, next) => {
  const { nom,email,tel, motdepasse, role } = req.body;

  try {
    const oldUser = await Admin.findOne({ email });

    if (oldUser) return res.status(400).json({ message: "User already exists" });

    const hashedPassword = await bcrypt.hash(motdepasse, 12);

    const result = await Admin.create({nom, email,tel, motdepasse: hashedPassword, role});

    //const token = jwt.sign( { email: result.email, id: result._id }, secret, { expiresIn: "1h" } );

    res.status(201).json({ result});
  } catch (error) {
    res.status(500).json({ message: "Something went wrong", content: error.message });
    
    console.log(error);
  }
};